#pragma once

#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

class InventorySystem
{
protected:

	struct ItemLayout
	{
		std::vector<ItemSlotPos> positions;
	};

	std::unordered_map<eItems, ItemLayout> items_layouts;

	std::vector<Inventory*> inventories;

public:

	InventorySystem()	{}
	~InventorySystem()	{}

	Inventory* CreateInventory(int32_t rows, int32_t cols);

	void RemoveInventory(Inventory* inventory);

	template <typename... T>
	void CreateItemLayout(eItems item, T... layouts_desc)
	{
		auto& layout = items_layouts[item];
		(layout.positions.push_back(layouts_desc), ...);
	}

	ItemLayout& GetItemLayout(eItems item) { return items_layouts[item]; }
};

extern InventorySystem* inventory_system;

#endif