/*
* Copyright(c) 2019 Intel Corporation
*
* This source code is subject to the terms of the BSD 2 Clause License and
* the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
* was not distributed with this source code in the LICENSE file, you can
* obtain it at https://www.aomedia.org/license/software-license. If the Alliance for Open
* Media Patent License 1.0 was not distributed with this source code in the
* PATENTS file, you can obtain it at https://www.aomedia.org/license/patent-license.
*/

#include <stdlib.h>
#include <string.h>

#include "EbThreads.h"
#include "EbReferenceObject.h"
#include "EbPictureBufferDesc.h"
#include "EbUtility.h"

// TODO: is this just padding with zeros? Is this needed?
void initialize_samples_neighboring_reference_picture16_bit(EbByte   recon_samples_buffer_ptr,
                                                            uint16_t stride, uint16_t recon_width,
                                                            uint16_t recon_height,
                                                            uint16_t left_padding,
                                                            uint16_t top_padding) {
    uint16_t *recon_samples_ptr;
    uint16_t  sample_count;

    // 1. zero out the top row
    recon_samples_ptr = (uint16_t *)recon_samples_buffer_ptr + (top_padding - 1) * stride +
        left_padding - 1;
    EB_MEMSET((uint8_t *)recon_samples_ptr, 0, sizeof(uint16_t) * (1 + recon_width + 1));

    // 2. zero out the bottom row
    recon_samples_ptr = (uint16_t *)recon_samples_buffer_ptr +
        (top_padding + recon_height) * stride + left_padding - 1;
    EB_MEMSET((uint8_t *)recon_samples_ptr, 0, sizeof(uint16_t) * (1 + recon_width + 1));

    // 3. zero out the left column
    recon_samples_ptr = (uint16_t *)recon_samples_buffer_ptr + top_padding * stride + left_padding -
        1;
    for (sample_count = 0; sample_count < recon_height; sample_count++)
        recon_samples_ptr[sample_count * stride] = 0;
    // 4. zero out the right column
    recon_samples_ptr = (uint16_t *)recon_samples_buffer_ptr + top_padding * stride + left_padding +
        recon_width;
    for (sample_count = 0; sample_count < recon_height; sample_count++)
        recon_samples_ptr[sample_count * stride] = 0;
}

void initialize_samples_neighboring_reference_picture_8bit(EbByte   recon_samples_buffer_ptr,
                                                           uint16_t stride, uint16_t recon_width,
                                                           uint16_t recon_height,
                                                           uint16_t left_padding,
                                                           uint16_t top_padding) {
    uint8_t *recon_samples_ptr;
    uint16_t sample_count;

    // 1. zero out the top row
    recon_samples_ptr = recon_samples_buffer_ptr + (top_padding - 1) * stride + left_padding - 1;
    EB_MEMSET(recon_samples_ptr, 0, sizeof(uint8_t) * (1 + recon_width + 1));

    // 2. zero out the bottom row
    recon_samples_ptr = recon_samples_buffer_ptr + (top_padding + recon_height) * stride +
        left_padding - 1;
    EB_MEMSET(recon_samples_ptr, 0, sizeof(uint8_t) * (1 + recon_width + 1));

    // 3. zero out the left column
    recon_samples_ptr = recon_samples_buffer_ptr + top_padding * stride + left_padding - 1;
    for (sample_count = 0; sample_count < recon_height; sample_count++)
        recon_samples_ptr[sample_count * stride] = 0;
    // 4. zero out the right column
    recon_samples_ptr = recon_samples_buffer_ptr + top_padding * stride + left_padding +
        recon_width;
    for (sample_count = 0; sample_count < recon_height; sample_count++)
        recon_samples_ptr[sample_count * stride] = 0;
}

void initialize_samples_neighboring_reference_picture(
    EbReferenceObject *          reference_object,
    EbPictureBufferDescInitData *picture_buffer_desc_init_data_ptr, EbBitDepthEnum bit_depth) {
    if (bit_depth == EB_10BIT) {
        initialize_samples_neighboring_reference_picture16_bit(
            reference_object->reference_picture16bit->buffer_y,
            reference_object->reference_picture16bit->stride_y,
            reference_object->reference_picture16bit->width,
            reference_object->reference_picture16bit->height,
            picture_buffer_desc_init_data_ptr->left_padding,
            picture_buffer_desc_init_data_ptr->top_padding);

        initialize_samples_neighboring_reference_picture16_bit(
            reference_object->reference_picture16bit->buffer_cb,
            reference_object->reference_picture16bit->stride_cb,
            reference_object->reference_picture16bit->width >> 1,
            reference_object->reference_picture16bit->height >> 1,
            picture_buffer_desc_init_data_ptr->left_padding >> 1,
            picture_buffer_desc_init_data_ptr->top_padding >> 1);

        initialize_samples_neighboring_reference_picture16_bit(
            reference_object->reference_picture16bit->buffer_cr,
            reference_object->reference_picture16bit->stride_cr,
            reference_object->reference_picture16bit->width >> 1,
            reference_object->reference_picture16bit->height >> 1,
            picture_buffer_desc_init_data_ptr->left_padding >> 1,
            picture_buffer_desc_init_data_ptr->top_padding >> 1);
    } else {
        initialize_samples_neighboring_reference_picture_8bit(
            reference_object->reference_picture->buffer_y,
            reference_object->reference_picture->stride_y,
            reference_object->reference_picture->width,
            reference_object->reference_picture->height,
            picture_buffer_desc_init_data_ptr->left_padding,
            picture_buffer_desc_init_data_ptr->top_padding);

        initialize_samples_neighboring_reference_picture_8bit(
            reference_object->reference_picture->buffer_cb,
            reference_object->reference_picture->stride_cb,
            reference_object->reference_picture->width >> 1,
            reference_object->reference_picture->height >> 1,
            picture_buffer_desc_init_data_ptr->left_padding >> 1,
            picture_buffer_desc_init_data_ptr->top_padding >> 1);

        initialize_samples_neighboring_reference_picture_8bit(
            reference_object->reference_picture->buffer_cr,
            reference_object->reference_picture->stride_cr,
            reference_object->reference_picture->width >> 1,
            reference_object->reference_picture->height >> 1,
            picture_buffer_desc_init_data_ptr->left_padding >> 1,
            picture_buffer_desc_init_data_ptr->top_padding >> 1);
    }
}

static void svt_reference_object_dctor(EbPtr p) {
    EbReferenceObject *obj = (EbReferenceObject *)p;
    EB_DELETE(obj->reference_picture16bit);
    EB_DELETE(obj->reference_picture);
    EB_FREE_ALIGNED_ARRAY(obj->mvs);
    EB_DESTROY_MUTEX(obj->referenced_area_mutex);

    for (uint8_t denom_idx = 0; denom_idx < NUM_SCALES; denom_idx++) {
        if (obj->downscaled_reference_picture[denom_idx] != NULL) {
            EB_DELETE(obj->downscaled_reference_picture[denom_idx]);
        }
        if (obj->downscaled_reference_picture16bit[denom_idx] != NULL) {
            EB_DELETE(obj->downscaled_reference_picture16bit[denom_idx]);
        }
    }
    EB_DELETE(obj->quarter_reference_picture);
    EB_DELETE(obj->sixteenth_reference_picture);
    EB_DELETE(obj->input_picture);
    EB_DELETE(obj->quarter_input_picture);
    EB_DELETE(obj->sixteenth_input_picture);
}

/*****************************************
 * svt_picture_buffer_desc_ctor
 *  Initializes the Buffer Descriptor's
 *  values that are fixed for the life of
 *  the descriptor.
 *****************************************/
EbErrorType svt_reference_object_ctor(EbReferenceObject *reference_object,
                                      EbPtr              object_init_data_ptr) {
    EbReferenceObjectDescInitData *ref_init_ptr = (EbReferenceObjectDescInitData *)
        object_init_data_ptr;
    EbPictureBufferDescInitData *picture_buffer_desc_init_data_ptr =
        &ref_init_ptr->reference_picture_desc_init_data;
    EbPictureBufferDescInitData picture_buffer_desc_init_data_16bit_ptr =
        *picture_buffer_desc_init_data_ptr;

    reference_object->dctor = svt_reference_object_dctor;
    //TODO:12bit
    if (picture_buffer_desc_init_data_16bit_ptr.bit_depth == EB_10BIT) {
        // Hsan: set split_mode to 0 to construct the packed reference buffer (used @ EP)
        picture_buffer_desc_init_data_16bit_ptr.split_mode = EB_FALSE;
        EB_NEW(reference_object->reference_picture16bit,
               svt_picture_buffer_desc_ctor,
               (EbPtr)&picture_buffer_desc_init_data_16bit_ptr);

        initialize_samples_neighboring_reference_picture(
            reference_object,
            &picture_buffer_desc_init_data_16bit_ptr,
            picture_buffer_desc_init_data_16bit_ptr.bit_depth);
        // Use 8bit here to use in MD
        picture_buffer_desc_init_data_16bit_ptr.split_mode = EB_FALSE;
        picture_buffer_desc_init_data_16bit_ptr.bit_depth  = EB_8BIT;

        // if hbd_md = 1, and we only use 8bit luma for obmc_motion_refine
        if (ref_init_ptr->hbd_mode_decision == EB_10_BIT_MD) {
            picture_buffer_desc_init_data_16bit_ptr.buffer_enable_mask =
                PICTURE_BUFFER_DESC_LUMA_MASK;
        }
        EB_NEW(reference_object->reference_picture,
               svt_picture_buffer_desc_ctor,
               (EbPtr)&picture_buffer_desc_init_data_16bit_ptr);
    } else {
        // Hsan: set split_mode to 0 to as 8BIT input
        picture_buffer_desc_init_data_ptr->split_mode = EB_FALSE;
        EB_NEW(reference_object->reference_picture,
               svt_picture_buffer_desc_ctor,
               (EbPtr)picture_buffer_desc_init_data_ptr);

        initialize_samples_neighboring_reference_picture(
            reference_object,
            picture_buffer_desc_init_data_ptr,
            picture_buffer_desc_init_data_16bit_ptr.bit_depth);
        if (picture_buffer_desc_init_data_ptr->is_16bit_pipeline) {
            picture_buffer_desc_init_data_16bit_ptr.split_mode = EB_FALSE;
            picture_buffer_desc_init_data_16bit_ptr.bit_depth  = EB_10BIT;
            EB_NEW(reference_object->reference_picture16bit,
                   svt_picture_buffer_desc_ctor,
                   (EbPtr)&picture_buffer_desc_init_data_16bit_ptr);

            initialize_samples_neighboring_reference_picture(
                reference_object,
                picture_buffer_desc_init_data_ptr,
                picture_buffer_desc_init_data_16bit_ptr.bit_depth);
            reference_object->reference_picture16bit->bit_depth = EB_8BIT;
        }
    }
    reference_object->input_picture = NULL;

    uint32_t mi_rows = reference_object->reference_picture->height >> MI_SIZE_LOG2;
    uint32_t mi_cols = reference_object->reference_picture->width >> MI_SIZE_LOG2;

    if (picture_buffer_desc_init_data_ptr->mfmv) {
        //MFMV map is 8x8 based.
        const int mem_size = ((mi_rows + 1) >> 1) * ((mi_cols + 1) >> 1);
        EB_CALLOC_ALIGNED_ARRAY(reference_object->mvs, mem_size);
    }
    reference_object->quarter_reference_picture   = NULL;
    reference_object->sixteenth_reference_picture = NULL;

    reference_object->ds_pics.picture_ptr           = reference_object->reference_picture;
    reference_object->ds_pics.quarter_picture_ptr   = reference_object->quarter_reference_picture;
    reference_object->ds_pics.sixteenth_picture_ptr = reference_object->sixteenth_reference_picture;
    memset(&reference_object->film_grain_params, 0, sizeof(reference_object->film_grain_params));
    EB_CREATE_MUTEX(reference_object->referenced_area_mutex);

    // set all supplemental downscaled reference picture pointers to NULL
    for (uint8_t down_idx = 0; down_idx < NUM_SCALES; down_idx++) {
        reference_object->downscaled_reference_picture[down_idx]      = NULL;
        reference_object->downscaled_reference_picture16bit[down_idx] = NULL;
    }

    reference_object->mi_rows = mi_rows;
    reference_object->mi_cols = mi_cols;

    return EB_ErrorNone;
}

EbErrorType svt_reference_object_creator(EbPtr *object_dbl_ptr, EbPtr object_init_data_ptr) {
    EbReferenceObject *obj;

    *object_dbl_ptr = NULL;
    EB_NEW(obj, svt_reference_object_ctor, object_init_data_ptr);
    *object_dbl_ptr = obj;

    return EB_ErrorNone;
}

static void svt_pa_reference_object_dctor(EbPtr p) {
    EbPaReferenceObject *obj = (EbPaReferenceObject *)p;
    if (obj->dummy_obj)
        return;
    EB_DELETE(obj->input_padded_picture_ptr);
    EB_DELETE(obj->quarter_downsampled_picture_ptr);
    EB_DELETE(obj->sixteenth_downsampled_picture_ptr);
    for (uint8_t denom_idx = 0; denom_idx < NUM_SCALES; denom_idx++) {
        if (obj->downscaled_input_padded_picture_ptr[denom_idx] != NULL) {
            EB_DELETE(obj->downscaled_input_padded_picture_ptr[denom_idx]);
            EB_DELETE(obj->downscaled_quarter_downsampled_picture_ptr[denom_idx]);
            EB_DELETE(obj->downscaled_sixteenth_downsampled_picture_ptr[denom_idx]);
        }
    }
}

/*****************************************
 * svt_pa_reference_object_ctor
 *  Initializes the Buffer Descriptor's
 *  values that are fixed for the life of
 *  the descriptor.
 *****************************************/
EbErrorType svt_pa_reference_object_ctor(EbPaReferenceObject *pa_ref_obj_,
                                         EbPtr                object_init_data_ptr) {
    EbPictureBufferDescInitData *picture_buffer_desc_init_data_ptr = (EbPictureBufferDescInitData *)
        object_init_data_ptr;

    pa_ref_obj_->dctor                                    = svt_pa_reference_object_dctor;

    // Reference picture constructor
    EB_NEW(pa_ref_obj_->input_padded_picture_ptr,
           svt_picture_buffer_desc_ctor,
           (EbPtr)picture_buffer_desc_init_data_ptr);
    // Downsampled reference picture constructor
    EB_NEW(pa_ref_obj_->quarter_downsampled_picture_ptr,
        svt_picture_buffer_desc_ctor,
        (EbPtr)(picture_buffer_desc_init_data_ptr + 1));
    EB_NEW(pa_ref_obj_->sixteenth_downsampled_picture_ptr,
        svt_picture_buffer_desc_ctor,
        (EbPtr)(picture_buffer_desc_init_data_ptr + 2));
    // set all supplemental downscaled reference picture pointers to NULL
    for (uint8_t down_idx = 0; down_idx < NUM_SCALES; down_idx++) {
        pa_ref_obj_->downscaled_input_padded_picture_ptr[down_idx]        = NULL;
        pa_ref_obj_->downscaled_quarter_downsampled_picture_ptr[down_idx] = NULL;
        pa_ref_obj_->downscaled_sixteenth_downsampled_picture_ptr[down_idx] = NULL;
    }

    return EB_ErrorNone;
}

EbErrorType svt_pa_reference_object_creator(EbPtr *object_dbl_ptr, EbPtr object_init_data_ptr) {
    EbPaReferenceObject *obj;

    *object_dbl_ptr = NULL;
    EB_NEW(obj, svt_pa_reference_object_ctor, object_init_data_ptr);
    *object_dbl_ptr = obj;

    return EB_ErrorNone;
}

/************************************************
* Release Pa Reference Objects
** Check if reference pictures are needed
** release them when appropriate
************************************************/
void release_pa_reference_objects(SequenceControlSet *scs_ptr, PictureParentControlSet *pcs_ptr) {
    // PA Reference Pictures
    if (pcs_ptr->slice_type != I_SLICE) {
        uint32_t num_of_list_to_search = (pcs_ptr->slice_type == P_SLICE) ? REF_LIST_0 : REF_LIST_1;

        // List Loop
        for (uint32_t list_index = REF_LIST_0; list_index <= num_of_list_to_search; ++list_index) {
            // Release PA Reference Pictures
            uint8_t num_of_ref_pic_to_search = (pcs_ptr->slice_type == P_SLICE)
                ? MIN(pcs_ptr->ref_list0_count, scs_ptr->reference_count)
                : (list_index == REF_LIST_0)
                ? MIN(pcs_ptr->ref_list0_count, scs_ptr->reference_count)
                : MIN(pcs_ptr->ref_list1_count, scs_ptr->reference_count);

            for (uint32_t ref_pic_index = 0; ref_pic_index < num_of_ref_pic_to_search;
                 ++ref_pic_index) {
                if (pcs_ptr->ref_pa_pic_ptr_array[list_index][ref_pic_index] != NULL) {
                    svt_release_object(pcs_ptr->ref_pa_pic_ptr_array[list_index][ref_pic_index]);
                }
            }
        }
    }

    if (pcs_ptr->pa_reference_picture_wrapper_ptr != NULL) {
        svt_release_object(pcs_ptr->pa_reference_picture_wrapper_ptr);
    }

    return;
}
