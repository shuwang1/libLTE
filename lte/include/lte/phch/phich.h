/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2014 The libLTE Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution.
 *
 * \section LICENSE
 *
 * This file is part of the libLTE library.
 *
 * libLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */


#ifndef PHICH_
#define PHICH_

#include "lte/common/base.h"
#include "lte/mimo/precoding.h"
#include "lte/mimo/layermap.h"
#include "lte/modem/mod.h"
#include "lte/modem/demod_hard.h"
#include "lte/scrambling/scrambling.h"
#include "regs.h"

typedef _Complex float cf_t;

#define PHICH_NORM_NSEQUENCES	8
#define PHICH_EXT_NSEQUENCES	4
#define PHICH_MAX_SEQUENCES		PHICH_NORM_NSEQUENCES
#define PHICH_NBITS				3

#define PHICH_NORM_MSYMB		PHICH_NBITS * 4
#define PHICH_EXT_MSYMB			PHICH_NBITS * 2
#define PHICH_MAX_NSYMB			PHICH_NORM_MSYMB
#define PHICH_NORM_C			1
#define PHICH_EXT_C				2
#define PHICH_NORM_NSF			4
#define PHICH_EXT_NSF			2

/* phich object */
typedef struct {
	lte_cp_t cp;
	int nof_prb;
	int nof_tx_ports;

	/* handler to REGs resource mapper */
	regs_t *regs;

	/* buffers */
	cf_t ce[MAX_PORTS_CTRL][PHICH_MAX_NSYMB];
	cf_t phich_symbols[MAX_PORTS_CTRL][PHICH_MAX_NSYMB];
	cf_t phich_x[MAX_PORTS_CTRL][PHICH_MAX_NSYMB];
	cf_t phich_d[PHICH_MAX_NSYMB];
	cf_t phich_d0[PHICH_MAX_NSYMB];
	cf_t phich_z[PHICH_NBITS];

	/* bit message */
	char data[PHICH_NBITS];

	/* tx & rx objects */
	modem_table_t mod;
	demod_hard_t demod;
	sequence_t seq_phich[NSUBFRAMES_X_FRAME];

}phich_t;

int phich_init(phich_t *q, regs_t *regs, int cell_id, int nof_prb, int nof_tx_ports, lte_cp_t cp);
void phich_free(phich_t *q);
int phich_decode(phich_t *q, cf_t *slot_symbols, cf_t *ce[MAX_PORTS_CTRL],
		int ngroup, int nseq, int nsubframe, char *ack, int *distance);
int phich_encode(phich_t *q, char ack, int ngroup, int nseq, int nsubframe,
		cf_t *slot_symbols[MAX_PORTS_CTRL]);


void phich_reset(phich_t *q, cf_t *slot_symbols[MAX_PORTS_CTRL]);
int phich_ngroups(phich_t *q);
bool phich_exists(int nframe, int nslot);
int phich_put(regs_t *h, cf_t *phich, cf_t *slot_data);
int phich_get(regs_t *h, cf_t *phich, cf_t *slot_data);

#endif
