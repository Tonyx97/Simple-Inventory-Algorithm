#pragma once

#ifndef INVENTORY_H
#define INVENTORY_H

#include "dbg.h"

enum eItems
{
	EMPTY_SLOT,
	MEDKIT,
	PISTOL,
	HAMMER
};

struct ItemSlotPos
{
	uint32_t row = UINT32_MAX, col = UINT32_MAX;
	ItemSlotPos()												{}
	ItemSlotPos(int32_t row, int32_t col) : row(row), col(col)	{}

	inline bool Equals(const ItemSlotPos& pos) const			{ return pos.row == row && pos.col == col; }
};

struct ItemSlot
{
	ItemSlotPos item_root_pos;
	eItems item_id = EMPTY_SLOT;

	ItemSlot() {}
	ItemSlot(const ItemSlotPos& item_root_pos, eItems item_id) : item_root_pos(item_root_pos), item_id(item_id) {}

	void Clear()
	{
		item_root_pos.row = -1;
		item_root_pos.col = -1;
		item_id = EMPTY_SLOT;
	}

	void CopyTo(ItemSlot* slot)
	{
		slot->item_root_pos = item_root_pos;
		slot->item_id = item_id;
	}

	inline bool IsEmpty() const					{ return item_id == EMPTY_SLOT; }
	inline bool IsSame(ItemSlot* slot) const	{ return slot->item_id == item_id && slot->item_root_pos.Equals(item_root_pos); }
};

struct Inventory
{
	uint32_t cols = 0, rows = 0;
	ItemSlot*** data = nullptr;
	std::vector<ItemSlot*> items;

	Inventory(int32_t rows, int32_t cols);
	~Inventory();

	inline bool IsPointInBound(const ItemSlotPos& pos)	{ return pos.row >= 0 && pos.col >= 0 && pos.row < rows && pos.col < cols; }

	bool CanItemBeInserted(eItems item, const ItemSlotPos& item_pos, std::vector<ItemSlot*>& occupied_slots);
	bool CanItemBeRemoved(eItems item, const ItemSlotPos& item_pos, std::vector<ItemSlot*>& occupied_slots);
	bool InsertItem(eItems item, int32_t row, int32_t col);
	bool RemoveItem(int32_t row, int32_t col);
	bool MoveItem(int32_t from_row, int32_t from_col, int32_t to_row, int32_t to_col);

	void Show();
	void LoopItems();
};

#endif