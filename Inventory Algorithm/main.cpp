#include <Windows.h>
#include <algorithm>
#include <random>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

#include "dbg.h"

#include "Inventory.h"
#include "InventorySystem.h"

void debug()
{
	while (true)
	{
		PROCESS_MEMORY_COUNTERS_EX data;
		GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&data), sizeof(data));
		float mb = static_cast<float>(data.WorkingSetSize) / powf(2.f, 20.f),
			kb = static_cast<float>(data.WorkingSetSize) / powf(2.f, 10.f);

		char buf[64] = { 0 };
		sprintf_s(buf, "Memory: %.3f MB | %.3f KB", mb, kb);
		SetConsoleTitleA(buf);
		Sleep(100);
	}
}

int main()
{
	std::thread dbg_thread(debug);
	std::mt19937 mt(GetTickCount64());

	inventory_system = new InventorySystem();

	inventory_system->CreateItemLayout(MEDKIT, ItemSlotPos{ 0, 0 }, ItemSlotPos{ -1, 0 }, ItemSlotPos{ 0, -1 }, ItemSlotPos{ 0, 1 }, ItemSlotPos{ 1, 0 });
	inventory_system->CreateItemLayout(PISTOL, ItemSlotPos{ 0, 0 }, ItemSlotPos{ 1, 0 }, ItemSlotPos{ 1, 1 }, ItemSlotPos{ 0, 1 });

	while (true)
	{
		int32_t inv_rows = 15,
				inv_cols = 10;

		Inventory* inv = inventory_system->CreateInventory(inv_rows, inv_cols);

		{
			int32_t row = rand() % 10, col = rand() % 10;
			eItems item = MEDKIT;

			bool inserted = inv->InsertItem(item, row, col);
			print(white, "Item %i inserted: %s (row: %i | col: %i)\n", item, inserted ? "Yes" : "No", row, col);

			//bool removed = inv->RemoveItem(1, 3);
			//print(white, "Item %i removed: %s (%i %i)\n", item, removed ? "Yes" : "No", row, col);
		}

		{
			int32_t row = rand() % 10, col = rand() % 10;
			eItems item = PISTOL;

			bool inserted = inv->InsertItem(item, row, col);
			print(white, "Item %i inserted: %s (row: %i | col: %i)\n", item, inserted ? "Yes" : "No", row, col);

			//bool removed = inv->RemoveItem(4, 5);
			//print(white, "Item %i removed: %s (%i %i)\n", item, removed ? "Yes" : "No", row, col);

			int32_t mrow = rand() % 10, mcol = rand() % 10;
			bool moved = inv->MoveItem(mrow, mcol, 10, 8);
			print(white, "Item %i moved to (%i %i): %s\n", item, mrow, mcol, moved ? "Yes" : "No");
			
		}

		inv->Show();
		inv->LoopItems();

		inventory_system->RemoveInventory(inv);

		std::cin.get();
	}

	delete inventory_system;

	return std::cin.get();
}