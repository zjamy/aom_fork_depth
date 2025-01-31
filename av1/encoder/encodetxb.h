/*
 * Copyright (c) 2017, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef AOM_AV1_ENCODER_ENCODETXB_H_
#define AOM_AV1_ENCODER_ENCODETXB_H_

#include "config/aom_config.h"

#include "av1/common/blockd.h"
#include "av1/common/onyxc_int.h"
#include "av1/common/txb_common.h"
#include "av1/encoder/block.h"
#include "av1/encoder/encoder.h"
#include "aom_dsp/bitwriter.h"
#ifdef __cplusplus
extern "C" {
#endif

#define TXB_SKIP_CTX_MASK 15
#define DC_SIGN_CTX_SHIFT 4
#define DC_SIGN_CTX_MASK 3

typedef struct TxbInfo {
  tran_low_t *qcoeff;
  uint8_t *levels;  // absolute values and clamped to 255.
  tran_low_t *dqcoeff;
  const tran_low_t *tcoeff;
  const int16_t *dequant;
  int shift;
  TX_SIZE tx_size;
  TX_SIZE txs_ctx;
  TX_TYPE tx_type;
  int bwl;
  int width;
  int height;
  int eob;
  int seg_eob;
  const SCAN_ORDER *scan_order;
  TXB_CTX *txb_ctx;
  int64_t rdmult;
  const qm_val_t *iqmatrix;
  int tx_type_cost;
} TxbInfo;

void av1_alloc_txb_buf(AV1_COMP *cpi);
void av1_free_txb_buf(AV1_COMP *cpi);
int av1_cost_coeffs_txb(const MACROBLOCK *x, const int plane, const int block,
                        const TX_SIZE tx_size, const TX_TYPE tx_type,
                        const TXB_CTX *const txb_ctx, int reduced_tx_set_used);
void av1_write_coeffs_txb(const AV1_COMMON *const cm, MACROBLOCK *const x,
                          aom_writer *w, int blk_row, int blk_col, int plane,
                          int block, TX_SIZE tx_size);
void av1_write_coeffs_mb(const AV1_COMMON *const cm, MACROBLOCK *x, int mi_row,
                         int mi_col, aom_writer *w, BLOCK_SIZE bsize);
int av1_get_txb_entropy_context(const tran_low_t *qcoeff,
                                const SCAN_ORDER *scan_order, int eob);
void av1_update_txb_context(const AV1_COMP *cpi, ThreadData *td,
                            RUN_TYPE dry_run, BLOCK_SIZE bsize, int *rate,
                            int mi_row, int mi_col, uint8_t allow_update_cdf);

void av1_update_txb_context_b(int plane, int block, int blk_row, int blk_col,
                              BLOCK_SIZE plane_bsize, TX_SIZE tx_size,
                              void *arg);

void av1_update_and_record_txb_context(int plane, int block, int blk_row,
                                       int blk_col, BLOCK_SIZE plane_bsize,
                                       TX_SIZE tx_size, void *arg);
#if CONFIG_HTB_TRELLIS
void hbt_destroy();
#endif  // CONFIG_HTB_TRELLIS
int av1_optimize_txb_new(const struct AV1_COMP *cpi, MACROBLOCK *x, int plane,
                         int block, TX_SIZE tx_size, TX_TYPE tx_type,
                         const TXB_CTX *const txb_ctx, int *rate_cost,
                         int sharpness, int fast_mode);

CB_COEFF_BUFFER *av1_get_cb_coeff_buffer(const struct AV1_COMP *cpi, int mi_row,
                                         int mi_col);

// These numbers are empirically obtained.
static const int plane_rd_mult[REF_TYPES][PLANE_TYPES] = {
  { 17, 13 },
  { 16, 10 },
};

#ifdef __cplusplus
}
#endif

#endif  // AOM_AV1_ENCODER_ENCODETXB_H_
