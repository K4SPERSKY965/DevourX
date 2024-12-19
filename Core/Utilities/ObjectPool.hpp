#pragma once

#include "pch-il2cpp.h"

#include <unordered_map>
#include <string>
#include <iostream>
#include "Utilities/Utilities.h"
#include "helpers.h"

namespace XSceneGameObjectPool {
    class SceneGameObjectPool {
    public:
        struct SceneObject {
            std::string name;
            app::GameObject* gameObject;
            std::vector<SceneObject> children;
        };
    private:
        std::vector<SceneObject> sceneHierarchy;

        void AddChildrenToHierarchy(app::GameObject* parent, std::vector<SceneObject>& children) {
            app::Transform* parentTransform = app::GameObject_get_transform(parent, nullptr);
            if (!parentTransform || XUtility::IsNull((app::Object_1*)parentTransform)) return;

            int childCount = app::Transform_get_childCount(parentTransform, nullptr);
            for (int i = 0; i < childCount; ++i) {
                app::Transform* childTransform = app::Transform_GetChild(parentTransform, i, nullptr);
                if (!childTransform || XUtility::IsNull((app::Object_1*)childTransform)) continue;

                app::GameObject* childGameObject = app::Component_get_gameObject((app::Component*)childTransform, nullptr);
                if (childGameObject && !XUtility::IsNull((app::Object_1*)childGameObject)) {
                    SceneObject childObject;
                    childObject.name = il2cppi_to_string(app::Object_1_get_name((app::Object_1*)childGameObject, nullptr));
                    childObject.gameObject = childGameObject;

                    children.push_back(childObject);

                    AddChildrenToHierarchy(childGameObject, childObject.children);
                }
            }
        }
    public:
        static SceneGameObjectPool* GetInstance() {
            static SceneGameObjectPool instance;
            return &instance;
        }

        void RefreshSceneObjects() {
            app::Scene activeScene = app::SceneManager_GetActiveScene(nullptr);
            if (&activeScene == nullptr) {
                std::cout << "Failed to get active scene.\n";
                return;
            }

            // Root nesneleri al
            int rootCount = app::Scene_get_rootCount(&activeScene, nullptr);
            if (rootCount <= 0) {
                std::cout << "No root objects found in the scene.\n";
                return;
            }

            app::GameObject__Array* rootObjects = app::Scene_GetRootGameObjects(&activeScene, nullptr);
            if (rootObjects == nullptr) {
                std::cout << "Root objects array is null.\n";
                return;
            }

            // Listeyi temizle ve yeni root objeleri ekle
            sceneHierarchy.clear();
            for (int i = 0; i < rootCount; ++i) {
                app::GameObject* root = rootObjects->vector[i];
                if (root && !XUtility::IsNull((app::Object_1*)root)) {
                    SceneObject rootObject;
                    rootObject.name = il2cppi_to_string(app::Object_1_get_name((app::Object_1*)root, nullptr));
                    rootObject.gameObject = root;

                    // Çocukları ekle
                    AddChildrenToHierarchy(root, rootObject.children);

                    sceneHierarchy.push_back(rootObject);
                }
            }

            std::cout << "Scene hierarchy refreshed. Total root objects: " << sceneHierarchy.size() << "\n";
        }

        const std::vector<SceneObject>& GetSceneHierarchy() const {
            return sceneHierarchy;
        }
    };
}

namespace XObjectPool {
    class ObjectPool {
    public:
        static ObjectPool* GetInstance() {
            static ObjectPool instance;
            return &instance;
        }

        void AddGameObject(const std::string& name, app::GameObject* go, const app::Vector3& pos, const app::Quaternion& rot) {
            objects[name] = { go, pos, rot };
        }

        void RemoveGameObject(const std::string& name) {
            std::cout << "Number of objects in map: " << objects.size() << "\n";
            if (objects.empty()) {
                std::cout << "The map is empty.\n";
            }
            else {
                const std::string targetKey = name;
                auto it = objects.find(targetKey);
                if (it != objects.end()) {
                    const auto& value = it->second;

                    std::cout << "Found Key: " << targetKey << "\n";
                    std::cout << "GameObject: " << (value.gameObject ? "Valid" : "Null") << "\n";
                    std::cout << "Position: (" << value.position.x
                        << ", " << value.position.y
                        << ", " << value.position.z << ")\n";
                }
                else {
                    std::cout << "Key not found: " << targetKey << "\n";
                }
            }
        }

        app::GameObject* GetGameObject(const std::string& name) {
            auto it = objects.find(name);
            if (it != objects.end()) {
                return it->second.gameObject;
            }
            return nullptr; // Bulunamadıysa null döner
        }

        auto& GetObjects() { return objects; }

    private:
        struct GameObjectData {
            app::GameObject* gameObject;
            app::Vector3 position;
            app::Quaternion rotation;
        };

        std::unordered_map<std::string, GameObjectData> objects;
    };
}