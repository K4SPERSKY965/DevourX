#pragma once

#include "pch-il2cpp.h"

#include <string>
#include <iostream>
#include "helpers.h"
#include "Utilities/Utilities.h"
#include "Players/PlayerManager.h"

namespace Resolver {
	class Bridge {
	private:
		Il2CppClass* currentKlass;             
		Il2CppImage* currentImage;             
		const char* targetClassName;           
		const char* targetClassNamespace;      
	private:
		const MethodInfo* GetMethodInfo(const char* methodName, int argsCount = 0) const {
			if (currentKlass == nullptr) {
				std::cout << "[Error] Current class is null!" << std::endl;
				return nullptr;
			}

			const MethodInfo* methodInfo = il2cpp_class_get_method_from_name(currentKlass, methodName, argsCount);
			if (!methodInfo) {
				std::cout << "[Error] MethodInfo for " << methodName << " not found!" << std::endl;
			}

			return methodInfo;
		}
	public:
		Bridge(const char* className, const char* classNamespace = "", const char* assemblyName = "Assembly-CSharp.dll") {
			this->currentKlass = nullptr;
			this->currentImage = nullptr;
			this->targetClassName = className;
			this->targetClassNamespace = classNamespace;

			const Il2CppAssembly* assembly = il2cpp_domain_assembly_open(il2cpp_domain_get(), assemblyName);
			if (!assembly) {
				std::cout << "[Error] Failed to open assembly: " << assemblyName << std::endl;
			}

			this->currentImage = assembly->image;
			this->currentKlass = il2cpp_class_from_name(this->currentImage, classNamespace, className);

			if (!this->currentKlass) {
				std::cout << "[Error] Failed to locate class: " << className << " in namespace: " << classNamespace << std::endl;
			}
		}

		template <typename T>
		T InvokeMethod(const char* methodName, int argsCount = 0, void** args = nullptr, void* targetObject = nullptr) const {
			const MethodInfo* methodInfo = GetMethodInfo(methodName, argsCount);
			if (!methodInfo) {
				std::cerr << "[Error] Failed to find method: " << methodName << std::endl;
				return T();
			}

			Il2CppException* exception = nullptr;
			Il2CppObject* result = il2cpp_runtime_invoke(methodInfo, targetObject, args, &exception);

			if (exception) {
				char exceptionMessage[1024];
				il2cpp_format_exception(exception, exceptionMessage, sizeof(exceptionMessage));
				std::cerr << "[Error] Exception occurred during method invocation: " << exceptionMessage << std::endl;
				return T();
			}

			if constexpr (std::is_same_v<T, void>) {
				return; // Eğer dönüş tipi void ise hiçbir şey döndürme
			}
			else if constexpr (std::is_pointer_v<T>) {
				return reinterpret_cast<T>(result); // Pointer türü döndür
			}
			else {
				// Value type için il2cpp_object_unbox kullanımı
				if (result) {
					void* unboxedResult = il2cpp_object_unbox(result);
					return *reinterpret_cast<T*>(unboxedResult);
				}
				else {
					std::cerr << "[Warning] Method returned null for: " << methodName << std::endl;
					return T();
				}
			}
		}
	};
}
namespace UnityAPI {

	// The `PropertyAccessor` class provides an interface to interact with Il2Cpp objects,
	// enabling access to classes, static properties, and instance properties in a given assembly.
	class PropertyAccessor {
	private:
		const Il2CppImage* currentImage;
	protected:
		/**
		 * @brief Finds an Il2Cpp class by its namespace and class name within the current image.
		 *
		 * This method is used to locate and retrieve a class definition from the Il2Cpp image.
		 * It performs checks to ensure the image, assembly, and class exist.
		 *
		 * @param namespaceName The namespace where the target class resides.
		 * @param className The name of the class to find.
		 * @return A pointer to the `Il2CppClass` if found; otherwise, nullptr.
		 */
		Il2CppClass* FindClass(const std::string& namespaceName, const std::string& className) {
			if (!currentImage) {
				std::cout << "[Error] Current image is null." << std::endl;
				return nullptr;
			}

			const Il2CppAssembly* assembly = currentImage->assembly;
			if (!assembly) {
				std::cout << "[Error] Assembly is null." << std::endl;
				return nullptr;
			}

			const Il2CppImage* image = il2cpp_assembly_get_image(assembly);
			if (!image) {
				std::cout << "[Error] Image is null." << std::endl;
				return nullptr;
			}

			Il2CppClass* klass = il2cpp_class_from_name(image, namespaceName.c_str(), className.c_str());
			if (!klass) {
				std::cout << "[Error] Class not found: " << namespaceName << "." << className << std::endl;
				return nullptr;
			}

			return klass;
		}

	public:
		const Il2CppImage* GetCurrentImage() const {
			return currentImage;
		}

		/**
		 * @brief Constructs the `PropertyAccessor` object, initializing the image for the specified assembly.
		 *
		 * This constructor initializes the current Il2Cpp image by opening the specified assembly
		 * in the current domain. The image is required for accessing classes and properties.
		 *
		 * @param assemblyName The name of the assembly to load (e.g., "UnityEngine.CoreModule.dll").
		 */
		PropertyAccessor(const char* assemblyName) {
			// Retrieve the Il2Cpp domain
			Il2CppDomain* domain = il2cpp_domain_get();
			if (!domain) {
				std::cout << "[ERROR] Il2Cpp domain not available.";
			}

			// Open the specified assembly and retrieve its image
			if (assemblyName && *assemblyName) {
				currentImage = il2cpp_domain_assembly_open(domain, assemblyName)->image;
				if (!currentImage) {
					std::cout << "[ERROR] Assembly not found: " + std::string(assemblyName);
				}
			}
		}

		/**
		 * @brief Retrieves the value of a static property from a given class.
		 *
		 * This method finds the class by its namespace and name, locates the getter method
		 * for the specified property, and invokes the method to retrieve the property value.
		 *
		 * @tparam T The expected type of the property value.
		 * @param classNamespace The namespace of the target class.
		 * @param className The name of the class containing the static property.
		 * @param propertyName The name of the property to retrieve.
		 * @return The value of the property as type `T`.
		 */
		template <typename T>
		inline T getStaticProperty(const std::string& classNamespace, const std::string& className, const std::string& propertyName) {
			Il2CppClass* klass = FindClass(classNamespace, className);
			if (!klass) {
				std::cout << "[Error] Class not found: " + classNamespace + "." + className;
				return T();
			}

			std::string getterName = "get_" + propertyName;
			const MethodInfo* getterMethod = il2cpp_class_get_method_from_name(klass, getterName.c_str(), 0);
			if (!getterMethod) {
				std::cout << "[Error] Getter method not found: " + propertyName;
				return T();
			}

			// Invoke the getter method to retrieve the property value
			Il2CppObject* result = il2cpp_runtime_invoke(getterMethod, nullptr, nullptr, nullptr);
			return ConvertResult<T>(result);
		}

		/**
		 * @brief Retrieves the value of an instance property from a given object.
		 *
		 * This method checks the validity of the provided instance, locates the getter
		 * method for the specified property, and invokes it to retrieve the property value.
		 *
		 * @tparam T The expected type of the property value.
		 * @param instance A pointer to the Il2CppObject instance.
		 * @param propertyName The name of the property to retrieve.
		 * @return The value of the property as type `T`.
		 */
		template <typename T>
		inline T getInstanceProperty(Il2CppObject* instance, const std::string& propertyName) {
			if (!instance) {
				std::cout << "[Error] Instance is null.";
			}

			Il2CppClass* klass = il2cpp_object_get_class(instance);
			if (!klass) {
				std::cout << "[Error] Class not found.";
			}

			std::string getterName = "get_" + propertyName;
			const MethodInfo* getterMethod = il2cpp_class_get_method_from_name(klass, getterName.c_str(), 0);
			if (!getterMethod) {
				std::cout << "[Error] Getter method not found: " + propertyName;
			}

			// Invoke the getter method to retrieve the property value
			Il2CppObject* result = il2cpp_runtime_invoke(getterMethod, instance, nullptr, nullptr);
			return ConvertResult<T>(result);
		}

		// Template method to invoke a static method
		template <typename T, typename... Args>
		inline T InvokeStaticMethod(const std::string& namespaceName, const std::string& className, const std::string& methodName, Args... args) {
			Il2CppClass* klass = FindClass(namespaceName, className);
			if (!klass) {
				std::cout << "[Error] Class not found: " << namespaceName << "." << className << std::endl;
				return T();
			}

			const MethodInfo* method = il2cpp_class_get_method_from_name(klass, methodName.c_str(), sizeof...(Args));
			if (!method) {
				std::cout << "[Error] Method not found: " << methodName << std::endl;
				return T();
			}

			void* methodArgs[] = { CastArgument(args)... }; // Use CastArgument to convert each argument
			Il2CppObject* result = il2cpp_runtime_invoke(method, nullptr, methodArgs, nullptr);

			return ConvertResult<T>(result);
		}

	private:
		template <typename T>
		inline T ConvertResult(Il2CppObject* result) {
			if (!result) {
				std::cout << "[Error] Null result!" << std::endl;
				if constexpr (std::is_same_v<T, std::string>) {
					return "";
				}
				return T();
			}

			if constexpr (std::is_same_v<T, std::string>) {
				// Convert Il2CppString to std::string
				Il2CppString* il2cppString = reinterpret_cast<Il2CppString*>(result);
				return il2cppi_to_string(il2cppString);
			}
			else if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>) {
				// Unbox primitive types or enums
				return *(T*)il2cpp_object_unbox(result);
			}
			else {
				// Return the result as a raw pointer for other types
				return reinterpret_cast<T>(result);
			}
		}

		template <typename Arg>
		inline void* CastArgument(Arg arg) {
			if constexpr (std::is_same_v<Arg, const char*> || std::is_same_v<Arg, char*>) {
				// Special case for strings
				return reinterpret_cast<void*>(il2cpp_string_new(arg));
			}
			else {
				// General case
				return const_cast<void*>(reinterpret_cast<const void*>(&arg));
			}
		}
	};


	class UApplication : public PropertyAccessor {
	public:
		UApplication() : PropertyAccessor("UnityEngine.CoreModule.dll") {}
	public:
		std::string ProductName() {
			return getStaticProperty<std::string>("UnityEngine", "Application", "productName");
		}
		std::string GetVersion() {
			return getStaticProperty<std::string>("UnityEngine", "Application", "version");
		}
		std::string GetUnityVersion() {
			return getStaticProperty<std::string>("UnityEngine", "Application", "unityVersion");
		}
	};

	class UTransform : public PropertyAccessor {
	public:
		UTransform(app::Transform* transform) : PropertyAccessor("UnityEngine.CoreModule.dll")
		{
			this->transform = transform;
		}
	private:
		app::Transform* transform;
	public:
		app::Vector3 GetPosition() {
			auto obj = getInstanceProperty<Il2CppObject*>(reinterpret_cast<Il2CppObject*>(transform), "position");

			if (!obj) {
				std::cout << "[Error] Null position object!" << std::endl;
				return app::Vector3{ 0.0f, 0.0f, 0.0f };
			}

			// Unbox ederek Vector3'e dönüştür
			return *(reinterpret_cast<app::Vector3*>(il2cpp_object_unbox(obj)));
		}
	};

	class UTime : public PropertyAccessor {
	public:
		UTime() : PropertyAccessor("UnityEngine.CoreModule.dll") {}
	public:
		float deltaTime() {
			return getStaticProperty<float>("UnityEngine", "Time", "deltaTime");
		}
	};

	class UObject {
	private:
		app::Object_1* foundObject;
	private:
		Il2CppDomain* domain;
		const Il2CppAssembly* assembly;
		Il2CppClass* klass;
		const char* assemblyName;
		const char* className;
		const char* classNamespace;
	public:
		UObject(const char* assemblyName = "Assembly-CSharp.dll") {
			foundObject = nullptr;

			domain = il2cpp_domain_get();
			this->assemblyName = assemblyName;

			if (IsDomainValid()) {
				assembly = il2cpp_domain_assembly_open(domain, assemblyName);
			}
		}
	private:
		bool IsDomainValid() {
			return domain ? true : false;
		}

		bool CanCall() {
			return domain != nullptr && assembly != nullptr;
		}

		void SetClass() {
			this->klass = il2cpp_class_from_name(assembly->image, classNamespace, className);
		}

		Il2CppClass* GetKlass() const {
			if (klass && klass->initialized) {
				return this->klass;
			}
		}

		bool IsObjectValid() {
			return (!XUtility::IsNull(foundObject) && foundObject != nullptr);
		}
	public:
		inline app::GameObject__Array* FindGameObjectsWithTag(const char* tag) {
			app::GameObject__Array* go_array_result = app::GameObject_FindGameObjectsWithTag(reinterpret_cast<app::String*>(il2cpp_string_new(tag)), nullptr);

			return go_array_result ? go_array_result : nullptr;
		}


		app::Component* GetComponentByName(app::GameObject* go, const char* type)
		{
			app::Component* component = app::GameObject_GetComponentByName(go, reinterpret_cast<app::String*>(il2cpp_string_new(type)), nullptr);

			return component ? component : nullptr;
		}

		template<typename T>
		inline T* FindObjectOfType(const char* className, const char* classNamespace = "") {
			if (IsObjectValid()) return reinterpret_cast<T*>(foundObject);

			if (!CanCall()) return nullptr;

			this->className = className;
			this->classNamespace = classNamespace;

			SetClass();

			// Getting the class type - retrieving the type information of the class (Il2CppType)
			const Il2CppType* type = il2cpp_class_get_type(this->klass);
			if (!type) {
				//std::cout << "Error: Failed to get class type (" << className << ")" << std::endl;
				return nullptr;
			}

			// Getting the type object - using the class type to get the corresponding type object in Unity
			Il2CppObject* typeObject = il2cpp_type_get_object(type);
			if (!typeObject) {
				//td::cout << "Error: Failed to get type object!" << std::endl;
				return nullptr;
			}

			// Calling Unity's FindObjectOfType method - this function finds the first object of the given type in the scene
			app::Object_1* cachedfoundObject = app::Object_1_FindObjectOfType(reinterpret_cast<app::Type*>(typeObject), nullptr);
			if (!cachedfoundObject || XUtility::IsNull(cachedfoundObject)) {
				return nullptr;
			}

			foundObject = cachedfoundObject;

			// Returning the found object cast to the specified type
			return reinterpret_cast<T*>(foundObject);
		}

		std::string GetObjectName(app::Object_1* obj) {
			return il2cppi_to_string(app::Object_1_GetName(obj, nullptr));
		}

		template<typename T>
		inline bool Enabled(T* behaviour) {
			return app::Behaviour_get_enabled((app::Behaviour*)behaviour, nullptr);
		}

		template<typename T>
		inline void Enabled(T* behaviour, bool value) {
			app::Behaviour_set_enabled((app::Behaviour*)behaviour, value, nullptr);
		}
	};
}
