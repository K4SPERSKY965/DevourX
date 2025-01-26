using DevourX.NET.Core.Settings;
using HarmonyLib;
using Il2Cpp;
using Il2CppBehaviorDesigner.Runtime.Tasks.UltimateCharacterController;
using MelonLoader;
using UnityEngine;

namespace DevourX.NET.Core.Utility
{
    public class Hooks
    {
        [HarmonyPatch(typeof(Il2Cpp.NolanBehaviour), "OnAttributeUpdateValue")]
        static class NolanBehaviour_UV
        {
            [HarmonyPrefix]
            static void Prefix(ref Il2CppOpsive.UltimateCharacterController.Traits.Attribute attribute)
            {
                if (Settings.Settings.unlimitedUVLight && attribute.m_Name == "Battery")
                {
                    attribute.m_Value = 100.0f;
                    return;
                }
            }
        }

        [HarmonyPatch(typeof(LockedInteractable), "CanInteract")]
        static class LockedInteractable_CanInteract_Patch
        {
            [HarmonyPrefix]
            static bool Prefix(LockedInteractable __instance, GameObject character, ref bool __result)
            {
                if (Settings.Settings.unlockDoors)
                {
                    __instance.isLocked = false;
                    __result = true;
                    return false;
                }

                return true;
            }
        }

        [HarmonyPatch(typeof(DevourInput), "GetLongPress")]
        static class DevourInput_GetLongPress_Patch
        {
            [HarmonyPrefix]
            static bool Prefix(DevourInput __instance, String name, ref float duration, bool waitForRelease)
            {
                if (Settings.Settings.disableLongInteract)
                {
                    duration = 0.0f;
                }

                return true;
            }
        }

        [HarmonyPatch(typeof(NolanBehaviour), "Update")]
        static class NolanBehaviour_Update_Patch
        {
            [HarmonyPrefix]
            static bool Prefix(NolanBehaviour __instance)
            {
                if (__instance.entity.IsOwner)
                {
                    if (Settings.Settings.fullbright)
                    {
                            Misc.EnableFullBright(__instance);
                    }

                    if (Settings.Settings.fastMovement)
                    {
                        __instance.speedChangeAbility.m_MaxSpeedChangeValue = Settings.Settings.fastMovementMultiplier;
                        __instance.speedChangeAbility.m_SpeedChangeMultiplier = Settings.Settings.fastMovementMultiplier;
                    }

                    if (Settings.Settings.fly)
                    {
                        Misc.Fly(__instance, Settings.Settings.flyMultiplier);
                    }
                }

                return true;
            }
        }
    }
}
