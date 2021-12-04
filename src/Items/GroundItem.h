#pragma once

#include "Items/Item.h"

namespace Items
{
	class GroundItems final :
		public Item
	{
	private:
		using super = Item;

	public:
		GroundItems() = delete;
		GroundItems(const GroundItems&) = delete;
		GroundItems(GroundItems&&) = default;

		 GroundItems(std::ptrdiff_t a_count, bool a_stealing, std::vector<RE::ObjectRefHandle> a_items) :
			super(a_count, a_stealing, { a_items.data(), a_items.size() }),
			_items(std::move(a_items))
		{}

		~GroundItems() = default;

		GroundItems& operator=(const GroundItems&) = delete;
		GroundItems& operator=(GroundItems&&) = default;

	protected:
		 void DoTake(RE::Actor& a_dst, std::ptrdiff_t a_count) override
		{
			auto toRemove = std::clamp<std::ptrdiff_t>(a_count, 0, Count());
			if (toRemove <= 0) {
				assert(false);
				return;
			}

			for (auto& handle : _items) {
				auto item = handle.get();
				if (item) {
					const auto xCount = std::clamp<std::ptrdiff_t>(item->extraList.GetCount(), 1, toRemove);
					a_dst.PickUpObject(item.get(), static_cast<std::int32_t>(xCount), false, true);
					toRemove -= xCount;

					if (toRemove <= 0) {
						break;
					}
				}
			}
		}

	private:
		std::vector<RE::ObjectRefHandle> _items;
	};
}
