/*
 * Generic show_mem() implementation
 *
 * Copyright (C) 2008 Johannes Weiner <hannes@saeurebad.de>
 * All code subject to the GPL version 2.
 */

#include <linux/mm.h>
#include <linux/quicklist.h>
#include <linux/cma.h>

/*
 * CUSTOM EDIT FOR CS680
 * Add custom function for displaying /proc/buddyinfo.
 */
static void show_buddyinfo(void) {
  struct zone *zone;
  printk("Zachary Kaplan: Custom BuddyInfo\n");

  for_each_populated_zone(zone) {
    unsigned int order;
    unsigned long nr[MAX_ORDER], flags;

    spin_lock_irqsave(&zone->lock, flags);
    for (order = 0; order < MAX_ORDER; ++order) {
      nr[order] = zone->free_area[order].nr_free;
    }
    spin_unlock_irqrestore(&zone->lock, flags);

    printk("Zachary Kaplan: Node %d, zone %8s", zone_to_nid(zone), zone->name);
    for (order = 0; order < MAX_ORDER; ++order) {
      printk(KERN_CONT "%7lu", nr[order]);
    }
    printk(KERN_CONT "\n");
  }
}

void show_mem(unsigned int filter, nodemask_t *nodemask)
{
	pg_data_t *pgdat;
	unsigned long total = 0, reserved = 0, highmem = 0;
  
  /*
   * CUSTOM EDIT FOR CS680
   * Prefix all printk's with my name.
   * Add call to custom method above.
   */

	printk("Zachary Kaplan: Mem-Info:\n");
	show_free_areas(filter, nodemask);

	for_each_online_pgdat(pgdat) {
		unsigned long flags;
		int zoneid;

		pgdat_resize_lock(pgdat, &flags);
		for (zoneid = 0; zoneid < MAX_NR_ZONES; zoneid++) {
			struct zone *zone = &pgdat->node_zones[zoneid];
			if (!populated_zone(zone))
				continue;

			total += zone->present_pages;
			reserved += zone->present_pages - zone->managed_pages;

			if (is_highmem_idx(zoneid))
				highmem += zone->present_pages;
		}
		pgdat_resize_unlock(pgdat, &flags);
	}

	printk("Zachary Kaplan: %lu pages RAM\n", total);
	printk("Zachary Kaplan: %lu pages HighMem/MovableOnly\n", highmem);
	printk("Zachary Kaplan: %lu pages reserved\n", reserved);
#ifdef CONFIG_CMA
	printk("Zachary Kaplan: %lu pages cma reserved\n", totalcma_pages);
#endif
#ifdef CONFIG_QUICKLIST
	printk("Zachary Kaplan: %lu pages in pagetable cache\n",
		quicklist_total_size());
#endif
#ifdef CONFIG_MEMORY_FAILURE
	printk("Zachary Kaplan: %lu pages hwpoisoned\n",
    atomic_long_read(&num_poisoned_pages));
#endif

  show_buddyinfo();
}
