using MelonLoader;
using UnityEngine;

namespace DevourX.NET.Core
{
    public class MelonMain: MonoBehaviour
    {
        public MelonMain(IntPtr ptr)
            : base(ptr)
        {
        }

        #region MENU

        private Settings.Settings _Settings;

        private string[] tabs = { "Settings", "Gameplay", "Object Pool", "Players" }; 
        private int selectedTab = 0; 

        #endregion

        public void Start()
        {
            _Settings = new Settings.Settings();

            MelonLogger.Warning("Made with by Jadis0x and ALittlePatate.");
            MelonLogger.Warning("Github : https://github.com/jadis0x/DevourX");
        }

        public void Update()
        {
            if (UnityEngine.Input.GetKeyDown(_Settings.bMenuToggleKey))
            {
                _Settings.bShowMenu = !_Settings.bShowMenu;
            }
        }

        public void OnGUI()
        {
            if (!_Settings.bShowMenu) return;

            float menuWidth = Screen.width * _Settings.menuWidthPercentage;
            float menuHeight = Screen.height * _Settings.menuHeightPercentage;
            float menuX = (Screen.width - menuWidth) / 2; 
            float menuY = (Screen.height - menuHeight) / 2;

            GUILayout.BeginArea(new Rect(menuX, menuY, menuWidth, menuHeight), GUI.skin.box);
            GUILayout.Label("<color=#63aba9>DevourX.NET by Jadis0x - v4.0.1</color>");

            GUILayout.BeginHorizontal();
            for (int i = 0; i < tabs.Length; i++)
            {
                if (i == selectedTab)
                {
                    if (GUILayout.Button(tabs[i], GUILayout.Height(30)))
                    {
                        selectedTab = i; 
                    }
                }
                else
                {
                    if (GUILayout.Button(tabs[i], GUILayout.Height(30)))
                    {
                        selectedTab = i; 
                    }
                }
            }
            GUILayout.EndHorizontal();

            GUILayout.Space(10);


            switch (selectedTab)
            {
                case 0: 
                    ShowSettingsTab();
                    break;

                case 1: 
                    ShowGameplayTab();
                    break;

                case 2: 
                    ShowObjectPoolTab();
                    break;

                case 3:
                    ShowPlayersTab();
                    break;
            }

            GUILayout.EndArea();
        }

        private void ShowPlayersTab()
        {
            GUILayout.Label("<color=#788686>Players</color>");
            GUILayout.Space(5);
        }

        private void ShowObjectPoolTab()
        {
            GUILayout.Label("<color=#788686>Object Pool</color>");
            GUILayout.Space(5);
        }

        private void ShowSettingsTab()
        {
            GUILayout.Label("<color=#788686>General Information</color>");
            GUILayout.Label($"FPS: <color=#56da7b>{1.0f / Time.deltaTime:F2}</color>");

            GUILayout.Space(5);

            GUILayout.Label($"Total Memory: <color=#56da7b>{System.GC.GetTotalMemory(false) / 1024 / 1024} MB</color>");

            GUILayout.Space(10);

            GUILayout.Label("<color=#788686>Settings</color>");

            GUILayout.Label("Menu On/Off Key:");
            _Settings.bMenuToggleKey = (KeyCode)System.Enum.Parse(typeof(KeyCode), GUILayout.TextField(_Settings.bMenuToggleKey.ToString()));
        }

        private void ShowGameplayTab()
        {
            GUILayout.Label("<color=#788686>Gameplay</color>");
            GUILayout.Space(5);
        }
    }
}
