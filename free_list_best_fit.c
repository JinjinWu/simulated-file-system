#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Implement the best fit algorithm to find free space for the
   simulated file data.
 */

int get_free_block(FS *fs, int size) {

    Freeblock *prev = NULL;
    Freeblock *curr = fs->freelist;

    Freeblock *best = NULL;
    Freeblock *best_prev = NULL;

    while (curr != NULL) {

	if (curr->length >= size) {
	    if (best == NULL || curr->length < best->length) {
		best = curr;
		best_prev = prev;
	    }
	}
        // iterate to next node in freelist
        prev = curr;
        curr = curr->next;
    }


    if (best != NULL) {
        int index = best->offset;
        best->offset += size;
        best->length -= size;
        // block is same size, so remove it
        if (best->length == 0) {
            if (best_prev != NULL) {
                // skip the curr node we're about to delete
                best_prev->next = best->next;
            } else {
                fs->freelist = best->next;
            }
            // free the memory for the now empty block
            free(curr);
        }
        return index;
    } else {
	// didn't find a suitable block
	return -1;
    }
}


