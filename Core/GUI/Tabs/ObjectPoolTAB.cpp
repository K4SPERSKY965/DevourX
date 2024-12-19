#include "pch-il2cpp.h"

#include "ObjectPoolTAB.h"
#include "GUI/GUITheme.h" 
#include "Settings/Settings.h"
#include <imgui/imgui.h>
#include <iostream>
#include <Utilities/ObjectPool.hpp>
#include "Players/PlayerManager.h"
#include "helpers.h"
#include <Utilities/Utilities.h>
#include "UnityResolver.hpp"

void ObjectPoolTAB::Render()
{
	if (ImGui::BeginTabItem("Object Pool")) {
		ImGui::Spacing();
		ImGui::Text("Destroying objects may negatively affect the game. Be careful what you destroy.");

		// Scrollable area for object pool items
		ImGui::BeginChild("ObjectPoolScrollingRegion", ImVec2(0, 400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		// Map'i non-const olarak alıyoruz
		auto& objMap = XObjectPool::ObjectPool::GetInstance()->GetObjects();
		if (objMap.empty()) {
			ImGui::Text("No objects found!");
		}
		else {
			// Kritik objeler listesi
			std::vector<std::string> criticalObjects = {
				"Steam Inventory Validator",
				"Survival Lobby Controller"
			};

			for (auto it = objMap.begin(); it != objMap.end(); ) {
				const std::string& objectName = it->first;
				auto& objectInfo = it->second;

				// Obje bilgilerini çizdir
				ImGui::Text("Name: %s", objectName.c_str());
				ImGui::Text("Position: %.2f, %.2f, %.2f",
					objectInfo.position.x,
					objectInfo.position.y,
					objectInfo.position.z);

				// Kritik obje kontrolü
				bool isCritical = std::find(criticalObjects.begin(), criticalObjects.end(), objectName) != criticalObjects.end();
				if (isCritical) {
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Critical Object - Cannot Modify");
				}
				else {
					// Destroy butonu
					if (ImGui::Button(("Destroy##" + objectName).c_str())) {
						if (!objectInfo.gameObject || XUtility::IsNull((app::Object_1*)objectInfo.gameObject)) {
							std::cout << "Invalid gameObject for: " << objectName << "\n";
							it = objMap.erase(it); // Geçersiz obje ise sil
						}
						else {
							app::BoltNetwork_Destroy(objectInfo.gameObject, nullptr);
							std::cout << "Destroyed object: " << objectName << "\n";
							it = objMap.erase(it); // Silinen objeyi map'ten kaldır
						}
						continue; // Bir sonraki elemana geç
					}

					// TP butonu
					if (ImGui::Button(("TP##" + objectName).c_str())) {
						app::GameObject* go = objectInfo.gameObject;
						if (!go || XUtility::IsNull((app::Object_1*)go)) {
							std::cout << "Invalid gameObject for teleport: " << objectName << "\n";
							++it;
							continue;
						}

						app::NolanBehaviour* nb = LocalPlayer::GetNolan();
						if (!nb || XUtility::IsNull((app::Object_1*)nb)) {
							std::cout << "Failed to retrieve local player.\n";
							++it;
							continue;
						}

						app::Transform* targetObjectTransform = app::GameObject_get_transform(go, nullptr);
						if (!targetObjectTransform || XUtility::IsNull((app::Object_1*)targetObjectTransform)) {
							std::cout << "Transform null for GameObject: " << objectName << "\n";
							++it;
							continue;
						}

						app::Vector3 position = app::Transform_get_position(targetObjectTransform, nullptr);
						app::Quaternion rotation = app::Quaternion_get_identity(nullptr);

						app::NolanBehaviour_TeleportTo(nb, position, rotation, false, nullptr);
						std::cout << "Teleported to object: " << objectName
							<< " at position: (" << position.x
							<< ", " << position.y
							<< ", " << position.z << ")\n";
					}

					// TP to yourself butonu
					if (ImGui::Button(("TP to yourself##" + objectName).c_str())) {
						app::GameObject* go = objectInfo.gameObject;
						if (!go || XUtility::IsNull((app::Object_1*)go)) {
							std::cout << "Invalid gameObject for TP to yourself: " << objectName << "\n";
							++it;
							continue;
						}

						app::NolanBehaviour* nb = LocalPlayer::GetNolan();
						if (!nb || XUtility::IsNull((app::Object_1*)nb)) {
							std::cout << "Failed to retrieve local player for TP to yourself.\n";
							++it;
							continue;
						}

						// Oyuncunun GameObject ve Transform bileşenlerini al
						app::GameObject* playerGameObject = app::Component_get_gameObject((app::Component*)nb, nullptr);
						if (!playerGameObject || XUtility::IsNull((app::Object_1*)playerGameObject)) {
							std::cout << "Failed to retrieve local player's GameObject.\n";
							++it;
							continue;
						}

						app::Transform* localPlayerTransform = app::GameObject_get_transform(playerGameObject, nullptr);
						if (!localPlayerTransform || XUtility::IsNull((app::Object_1*)localPlayerTransform)) {
							std::cout << "Failed to get local player's transform.\n";
							++it;
							continue;
						}

						// Oyuncunun pozisyonunu al
						app::Vector3 playerPosition = app::Transform_get_position(localPlayerTransform, nullptr);

						// Objenin Transform bileşenini al ve pozisyonunu değiştir
						app::Transform* targetObjectTransform = app::GameObject_get_transform(go, nullptr);
						if (!targetObjectTransform || XUtility::IsNull((app::Object_1*)targetObjectTransform)) {
							std::cout << "Transform null for GameObject during TP to yourself: " << objectName << "\n";
							++it;
							continue;
						}

						app::Transform_set_position(targetObjectTransform, playerPosition, nullptr);
						std::cout << "Teleported object: " << objectName
							<< " to your position: (" << playerPosition.x
							<< ", " << playerPosition.y
							<< ", " << playerPosition.z << ")\n";
					}
				}

				// Separator çiz
				ImGui::Separator();

				// Sonraki elemana geç
				++it;
			}
		}

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}