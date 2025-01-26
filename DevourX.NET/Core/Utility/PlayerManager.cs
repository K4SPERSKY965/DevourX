using UnityEngine;

namespace DevourX.NET.Core.Utility
{
    public class PlayerManager
    {
        private static PlayerManager _instance;
        private readonly List<GameObject> _playersList = new List<GameObject>();
        private readonly HashSet<GameObject> _playersSet = new HashSet<GameObject>();
        private GameObject? _localPlayer;

        private PlayerManager() { }
        public static PlayerManager Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new PlayerManager();
                }
                return _instance;
            }
        }

        /// <summary>
        /// Gets all players currently in the game.
        /// </summary>
        public List<GameObject> GetAllPlayers()
        {
            if (_playersList.Count == 0)
            {
                RefreshPlayers();
            }

            // Remove null objects from the list and set
            _playersList.RemoveAll(player => player == null);
            _playersSet.RemoveWhere(player => player == null);

            // Find new players and add them to the list and set
            GameObject[] currentPlayers = GameObject.FindGameObjectsWithTag("Player");
            foreach (GameObject player in currentPlayers)
            {
                if (player != null && !_playersSet.Contains(player))
                {
                    _playersSet.Add(player);
                    _playersList.Add(player);
                }
            }

            return _playersList;
        }

        /// <summary>
        /// Refreshes the players list by clearing and re-fetching all players.
        /// </summary>
        public void RefreshPlayers()
        {
            _playersSet.Clear();
            _playersList.Clear();

            GameObject[] players = GameObject.FindGameObjectsWithTag("Player");
            foreach (GameObject player in players)
            {
                if (player != null)
                {
                    _playersSet.Add(player);
                    _playersList.Add(player);
                }
            }

            // Reset local player cache
            _localPlayer = null;
        }

        /// <summary>
        /// Gets the local player. Uses a cached value if available.
        /// </summary>
        public GameObject GetLocalPlayer()
        {
            if (_localPlayer != null && _localPlayer != null)
            {
                return _localPlayer;
            }

            foreach (GameObject player in GetAllPlayers())
            {
                Il2Cpp.NolanBehaviour nb = player.GetComponent<Il2Cpp.NolanBehaviour>();
                if (nb != null && nb.entity.IsOwner)
                {
                    _localPlayer = player;
                    break;
                }
            }

            return _localPlayer;
        }
    }
}
