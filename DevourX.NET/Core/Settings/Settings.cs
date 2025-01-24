using UnityEngine;

namespace DevourX.NET.Core.Settings
{
    public class Settings
    {
        public bool bShowMenu { get; set; } = false;
        public KeyCode bMenuToggleKey { get; set; } = KeyCode.F12;
        public readonly float menuWidthPercentage = 0.3f;  // Menu width is 30% of the screen width
        public readonly float menuHeightPercentage = 0.8f; // Menu height is 80% of the screen height
    }
}
