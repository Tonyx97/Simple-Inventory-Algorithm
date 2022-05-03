#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Inventory.h"
#include "InventorySystem.h"

Inventory::Inventory(int32_t rows, int32_t cols) : rows(rows), cols(cols)
{
	if (rows <= 0 || cols <= 0)
		return;

	data = new ItemSlot**[rows];
	for (int32_t i = 0; i < rows; ++i)
	{
		data[i] = new ItemSlot*[cols];
		for (int32_t j = 0; j < cols; ++j)
			data[i][j] = new ItemSlot();
	}
}

Inventory::~Inventory()
{
	for (auto&& item : items)
		delete item;
	for (int32_t i = 0; i < rows; ++i)
	{
		for (int32_t j = 0; j < cols; ++j)
			delete data[i][j];
		delete[] data[i];
	}
	delete[] data;
}

bool Inventory::CanItemBeInserted(eItems item, const ItemSlotPos& item_pos, std::vector<ItemSlot*>& occupied_slots)
{
	for (auto&& pos : inventory_system->GetItemLayout(item).positions)
	{
		ItemSlotPos slot_pos(item_pos.row + pos.row, item_pos.col + pos.col);
		if (!IsPointInBound(slot_pos))
			return false;

		ItemSlot* slot = data[slot_pos.row][slot_pos.col];
		if (!slot->IsEmpty())
			return false;

		occupied_slots.push_back(slot);
	}

	return true;
}

bool Inventory::CanItemBeRemoved(eItems item, const ItemSlotPos& item_pos, std::vector<ItemSlot*>& occupied_slots)
{
	if (!IsPointInBound(item_pos))
		return false;

	if (data[item_pos.row][item_pos.col]->IsEmpty())
		return false;

	for (auto&& pos : inventory_system->GetItemLayout(item).positions)
	{
		ItemSlotPos slot_pos(item_pos.row + pos.row, item_pos.col + pos.col);
		if (!IsPointInBound(slot_pos))
			return false;

		ItemSlot* slot = data[slot_pos.row][slot_pos.col];
		if (slot->IsEmpty())
			return false;

		occupied_slots.push_back(slot);
	}

	return true;
}

bool Inventory::InsertItem(eItems item, int32_t row, int32_t col)
{
	std::vector<ItemSlot*> occupied_slots;

	ItemSlotPos item_root_pos(row, col);
	if (!CanItemBeInserted(item, item_root_pos, occupied_slots))
		return false;

	ItemSlot* item_root_slot = new ItemSlot(item_root_pos, item);

	items.push_back(item_root_slot);

	for (auto& slot : occupied_slots)
		item_root_slot->CopyTo(slot);

	return true;
}

bool Inventory::RemoveItem(int32_t row, int32_t col)
{
	ItemSlot* selected_slot = data[row][col];
	ItemSlotPos item_root_pos = selected_slot->item_root_pos;

	std::vector<ItemSlot*> occupied_slots;

	if (!CanItemBeRemoved(selected_slot->item_id, item_root_pos, occupied_slots))
		return false;

	auto it = std::find_if(items.begin(), items.end(), [selected_slot](ItemSlot* slot) { return slot->IsSame(selected_slot); });
	if (it != items.end())
	{
		delete *it;
		items.erase(it);
	}

	for (auto& slot : occupied_slots)
		slot->Clear();
	
	return true;
}

bool Inventory::MoveItem(int32_t from_row, int32_t from_col, int32_t to_row, int32_t to_col)
{
	ItemSlot* selected_slot = data[from_row][from_col],
			* new_item_root_slot = data[to_row][to_col];
	ItemSlotPos item_root_pos = selected_slot->item_root_pos,
				new_item_root_pos = ItemSlotPos(to_row, to_col);

	std::vector<ItemSlot*> slots_to_clean, slots_to_fill;

	if (!CanItemBeRemoved(selected_slot->item_id, item_root_pos, slots_to_clean))
		return false;

	if (!CanItemBeInserted(selected_slot->item_id, new_item_root_pos, slots_to_fill))
		return false;

	new_item_root_slot->item_root_pos = new_item_root_pos;
	new_item_root_slot->item_id = selected_slot->item_id;

	for (auto& slot : slots_to_fill)
		new_item_root_slot->CopyTo(slot);

	for (auto& slot : slots_to_clean)
		slot->Clear();

	return true;
}

void Inventory::Show()
{
	for (uint32_t i = 0; i < rows; ++i)
	{
		for (uint32_t j = 0; j < cols; ++j)
		{
			auto d = data[i][j];
			if (d->item_root_pos.Equals(ItemSlotPos(i, j)))
				print(red, "%i ", d->item_id);
			else
			{
				if (d->item_id == 0)
					print(white, "%i ", d->item_id);
				else print(green, "%i ", d->item_id);
			}
		}
		print(white, "\n");
	}
}

void Inventory::LoopItems()
{
	for (auto&& item : items)
		printf_s("Item %i in slot (%i, %i)\n", item->item_id, item->item_root_pos.row, item->item_root_pos.col);
}