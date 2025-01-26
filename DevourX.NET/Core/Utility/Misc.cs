using Il2Cpp;
using Il2CppOpsive.UltimateCharacterController.Character;
using Il2CppPhoton.Bolt;
using UnityEngine;

namespace DevourX.NET.Core.Utility
{
    public class Misc
    {
        public static void ForceLobbyStart()
        {
            if (!GameHelper.IsLobbyOwner()) return;

            Il2CppHorror.Menu HorrorMenu = UnityEngine.Object.FindObjectOfType<Il2CppHorror.Menu>();
            if (HorrorMenu == null) return;

            HorrorMenu.OnLobbyStartButtonClick();
        }

        public static void EnableMovement(bool _enabled)
        {
            UnityEngine.GameObject LocalPlayer = PlayerManager.Instance.GetLocalPlayer();

            if (LocalPlayer == null) return;

            Il2CppOpsive.UltimateCharacterController.Character.UltimateCharacterLocomotionHandler LocomotionHandler = LocalPlayer.GetComponent<UltimateCharacterLocomotionHandler>();

            if (LocomotionHandler == null)
            {
                LocalPlayer.AddComponent<UltimateCharacterLocomotionHandler>();
            }
            else
            {
                LocomotionHandler.enabled = _enabled;
            }
        }

        public static void EnableFullBright(NolanBehaviour playerNolan)
        {
            if (playerNolan == null) return;

            Light light = playerNolan.flashlightSpot;

            if (light == null) return;

            light.intensity = 1.3f;
            light.spotAngle = 290f;
            light.range = 370f;

            light.shadows = LightShadows.None;
        }

        public static void Fly(NolanBehaviour playerNolanBehaviour, float speed)
        {
            Vector3 playerPosition = playerNolanBehaviour.transform.position;

            RewiredHelpers _RewiredHelpers = UnityEngine.Object.FindObjectOfType<RewiredHelpers>();
            if (_RewiredHelpers == null) return;

            if (Input.GetKey((KeyCode)System.Enum.Parse(typeof(KeyCode), _RewiredHelpers.GetCurrentBinding("Move Up").ToString().Replace(" ", ""))))
            {
                playerPosition += playerNolanBehaviour.transform.forward * speed * Time.deltaTime;
            }
            if (Input.GetKey((KeyCode)System.Enum.Parse(typeof(KeyCode), _RewiredHelpers.GetCurrentBinding("Move Down").ToString().Replace(" ", ""))))
            {
                playerPosition += -playerNolanBehaviour.transform.forward * speed * Time.deltaTime;
            }
            if (Input.GetKey((KeyCode)System.Enum.Parse(typeof(KeyCode), _RewiredHelpers.GetCurrentBinding("Move Right").ToString().Replace(" ", ""))))
            {
                playerPosition += playerNolanBehaviour.transform.right * speed * Time.deltaTime;
            }
            if (Input.GetKey((KeyCode)System.Enum.Parse(typeof(KeyCode), _RewiredHelpers.GetCurrentBinding("Move Left").ToString().Replace(" ", ""))))
            {
                playerPosition += -playerNolanBehaviour.transform.right * speed * Time.deltaTime;
            }
            if (Input.GetKey(KeyCode.Space))
            {
                playerPosition += playerNolanBehaviour.transform.up * speed * Time.deltaTime;
            }
            if (Input.GetKey(KeyCode.LeftControl))
            {
                playerPosition += -playerNolanBehaviour.transform.up * speed * Time.deltaTime;
            }

            playerNolanBehaviour.locomotion.SetPosition(playerPosition, false);
        }

        public static void KillPlayer(GameObject player)
        {
            /*if (player == null) return;

            BoltEntity component = player.GetComponent<BoltEntity>();
            IPlayerState state = component.GetState<IPlayerState>();

            state.Poison = 10000;
            */

            NolanBehaviour component = player.GetComponent<NolanBehaviour>();

            BoltEntity component2 = component.gameObject.GetComponent<BoltEntity>();
            KnockoutBoxEvent obj = KnockoutBoxEvent.Create();
            obj.KilledBy = player.name;
            obj.Player = component2;
            obj.Send();
        }

        public static void RevivePlayer(GameObject player)
        {
            BoltEntity cachedComponent = player.GetComponent<BoltEntity>();

            IPlayerState state = cachedComponent.GetState<IPlayerState>();

            BoltEntity player2 = cachedComponent;
            ReviveEvent obj = ReviveEvent.Create();
            obj.Player = player2;
            obj.Reviver = cachedComponent;
            obj.Send();

            // local only
            //Il2CppOpsive.Shared.Events.EventHandler.ExecuteEvent("OnResetGame");
        }

        public static void JumpscarePlayer(GameObject player)
        {
            
        }

        public static void CagePlayer(GameObject player)
        {
            if (player == null) return;

            BoltNetwork.Instantiate(BoltPrefabs.Cage, player.transform.position, Quaternion.identity);
        }

        public static void TeleportPlayer(GameObject player)
        {
            GameObject localPlayer = PlayerManager.Instance.GetLocalPlayer();

            if (localPlayer == null || player == null) return;

            localPlayer.GetComponent<NolanBehaviour>().TeleportTo(player.transform.position, Quaternion.identity, false);
        }

        public static void SpawnPrefab(PrefabId prefab, float spawnDistance)
        {
            GameObject localPlayer = PlayerManager.Instance.GetLocalPlayer();
            if (localPlayer != null)
            {
                SpawnAtTarget(prefab, localPlayer, spawnDistance);
            }
        }

        public static void SpawnPrefab(PrefabId prefab, GameObject target, float spawnDistance)
        {
            if (target != null)
            {
                SpawnAtTarget(prefab, target, spawnDistance);
            }
        }

        private static void SpawnAtTarget(PrefabId prefab, GameObject target, float spawnDistance)
        {
            Vector3 origin = target.transform.position;
            Vector3 forwardDirection = target.transform.forward;

            Vector3 spawnPosition = origin + forwardDirection * spawnDistance;

            BoltNetwork.Instantiate(prefab, spawnPosition, Quaternion.identity);
        }
    }
}
