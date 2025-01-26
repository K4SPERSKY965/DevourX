
using Il2CppPhoton.Bolt;

namespace DevourX.NET.Core.Utility
{
    public struct ObjectPool
    {
        public PrefabId Prefab;
        public string DisplayName;

        public ObjectPool(PrefabId prefab, string displayName)
        {
            Prefab = prefab;
            DisplayName = displayName;
        }
    }
}
