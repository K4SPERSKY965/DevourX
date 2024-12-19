#pragma once

namespace Misc {
	void FullBright(app::NolanBehaviour* localPlayer_nolanBehaviour);
	void ForceLobbyStart();
	void InstantWin();
	void CarryItem(const char* itemName);
	void CarryAnimal(const char* animalName);

	// events
	void Kill(app::GameObject* targetPlayer);
	void Revive(app::GameObject* targetPlayer);
	void Jumpscare(app::GameObject* targetPlayer);
}