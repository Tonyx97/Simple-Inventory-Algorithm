#include <stdint.h>
#include <vector>
#include <unordered_map>

#include "Inventory.h"
#include "InventorySystem.h"

InventorySystem* inventory_system = nullptr;

Inventory* InventorySystem::CreateInventory(int32_t rows, int32_t cols)
{
	auto inventory = new Inventory(rows, cols);
	inventories.push_back(inventory);
	return inventory;
}

void InventorySystem::RemoveInventory(Inventory* inventory)
{
	auto it = std::find(inventories.begin(), inventories.end(), inventory);
	if (it != inventories.end())
	{
		delete *it;
		inventories.erase(it);
	}
}