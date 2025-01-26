using Il2CppPhoton.Bolt;
using UnityEngine;

namespace DevourX.NET.Core.Utility
{
    public class GameHelper
    {
        public static bool IsLobbyOwner()
        {
            return BoltNetwork.IsServer;
        }

        public static string GetActiveSceneName()
        {
            return UnityEngine.SceneManagement.SceneManager.GetActiveScene().name;
        }

        public static string GetMapName(string sceneName)
        {
            switch (sceneName)
            {
                case "Devour":
                    return "Farmhouse";
                case "Molly":
                    return "Asylum";
                case "Inn":
                    return "Inn";
                case "Town":
                    return "Town";
                case "Slaughterhouse":
                    return "Slaughterhouse";
                case "Manor":
                    return "Manor";
                default:
                    return "Menu";
            }
        }

        public static BoltEntity GetLocalPlayerBoltEntity()
        {
            UnityEngine.GameObject playerGO = PlayerManager.Instance.GetLocalPlayer();
            if (playerGO == null) return null;

            return playerGO.GetComponent<BoltEntity>();
        }

        public static IPlayerState GetLocalPlayerState()
        {
            if (GetLocalPlayerBoltEntity() == null) return null;

            return GetLocalPlayerBoltEntity().GetState<IPlayerState>();
        }

        public static Texture2D MakeTex(int width, int height, Color color)
        {
            Texture2D texture = new Texture2D(width, height, TextureFormat.RGBA32, false);
            texture.SetPixel(0, 0, color);
            texture.Apply();
            return texture;
        }
    }
}
