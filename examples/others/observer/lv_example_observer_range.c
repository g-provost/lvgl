/**
 * @file lv_example_observer_range.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_examples.h"
#if LV_USE_OBSERVER && LV_BUILD_EXAMPLES

/*********************
 *      DEFINES
 *********************/

/*********************
 *      TYPEDEFS
 **********************/

/*********************
 *  STATIC PROTOTYPES
 **********************/
static void set_range_0_50_cb(lv_event_t * e);
static void set_range_25_75_cb(lv_event_t * e);
static void remove_range_cb(lv_event_t * e);

/*********************
 *  STATIC VARIABLES
 **********************/
static lv_subject_t * dynamic_subject = NULL;
static lv_obj_t * dynamic_range_label = NULL;

/*********************
 *      MACROS
 **********************/

/*********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Show basic usage of subjects with min-max ranges
 */
void lv_example_observer_range_basic(void)
{
    lv_obj_t * main_obj = lv_obj_create(lv_screen_active());
    lv_obj_set_size(main_obj, 300, 200);
    lv_obj_center(main_obj);

    /* Create an integer subject with range 0-100 */
    static lv_subject_t int_subject;
    lv_subject_init_int_range(&int_subject, 50, 0, 100);

    /* Create labels to show the value and range */
    lv_obj_t * value_label = lv_label_create(main_obj);
    lv_label_set_text(value_label, "Value: 50");
    lv_obj_align(value_label, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * range_label = lv_label_create(main_obj);
    lv_label_set_text_fmt(range_label, "Range: %d - %d", 
                          lv_subject_get_int_min(&int_subject),
                          lv_subject_get_int_max(&int_subject));
    lv_obj_align_to(range_label, value_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    /* Bind the label to show the current value */
    lv_label_bind_text(value_label, &int_subject, "Value: %d");

    /* Create a slider that's bound to the subject */
    lv_obj_t * slider = lv_slider_create(main_obj);
    lv_obj_align_to(slider, range_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_bind_value(slider, &int_subject);

    /* Create buttons to test out-of-range values */
    lv_obj_t * btn_low = lv_button_create(main_obj);
    lv_obj_align_to(btn_low, slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_t * label_low = lv_label_create(btn_low);
    lv_label_set_text(label_low, "Set -10");
    lv_obj_center(label_low);

    lv_obj_t * btn_high = lv_button_create(main_obj);
    lv_obj_align_to(btn_high, slider, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 10);
    lv_obj_t * label_high = lv_label_create(btn_high);
    lv_label_set_text(label_high, "Set 150");
    lv_obj_center(label_high);

    /* Add events to test range clamping */
    lv_obj_add_subject_set_int_event(btn_low, &int_subject, LV_EVENT_CLICKED, -10);  /* Will be clamped to 0 */
    lv_obj_add_subject_set_int_event(btn_high, &int_subject, LV_EVENT_CLICKED, 150); /* Will be clamped to 100 */
}

#if LV_USE_FLOAT
/**
 * Show float subject with range constraints
 */
void lv_example_observer_range_float(void)
{
    lv_obj_t * main_obj = lv_obj_create(lv_screen_active());
    lv_obj_set_size(main_obj, 300, 120);
    lv_obj_center(main_obj);

    /* Create a float subject with range -1.0 to 1.0 */
    static lv_subject_t float_subject;
    lv_subject_init_float_range(&float_subject, 0.0f, -1.0f, 1.0f);

    /* Create a label to show the value */
    lv_obj_t * value_label = lv_label_create(main_obj);
    lv_label_bind_text(value_label, &float_subject, "Float: %.2f");
    lv_obj_align(value_label, LV_ALIGN_TOP_MID, 0, 10);

    /* Create a slider for the float value (scaled to 0-200 for UI) */
    lv_obj_t * slider = lv_slider_create(main_obj);
    lv_obj_align_to(slider, value_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_slider_set_range(slider, -100, 100);
    lv_slider_bind_value(slider, &float_subject);

    /* Add range info */
    lv_obj_t * range_label = lv_label_create(main_obj);
    lv_label_set_text_fmt(range_label, "Range: %.1f - %.1f", 
                          lv_subject_get_float_min(&float_subject),
                          lv_subject_get_float_max(&float_subject));
    lv_obj_align_to(range_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
}
#endif /*LV_USE_FLOAT*/

/**
 * Show how to modify ranges dynamically
 */
void lv_example_observer_range_dynamic(void)
{
    lv_obj_t * main_obj = lv_obj_create(lv_screen_active());
    lv_obj_set_size(main_obj, 350, 250);
    lv_obj_center(main_obj);

    /* Create an integer subject without initial range */
    static lv_subject_t int_subject;
    lv_subject_init_int(&int_subject, 50);
    
    /* Store reference for callbacks */
    dynamic_subject = &int_subject;

    /* Create labels */
    lv_obj_t * value_label = lv_label_create(main_obj);
    lv_label_bind_text(value_label, &int_subject, "Value: %d");
    lv_obj_align(value_label, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * range_label = lv_label_create(main_obj);
    lv_label_set_text(range_label, "No range set");
    lv_obj_align_to(range_label, value_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    
    /* Store reference for callbacks */
    dynamic_range_label = range_label;

    /* Create slider */
    lv_obj_t * slider = lv_slider_create(main_obj);
    lv_obj_align_to(slider, range_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_bind_value(slider, &int_subject);

    /* Buttons to set different ranges */
    lv_obj_t * btn_range1 = lv_button_create(main_obj);
    lv_obj_set_size(btn_range1, 100, 30);
    lv_obj_align_to(btn_range1, slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_t * label1 = lv_label_create(btn_range1);
    lv_label_set_text(label1, "0-50");
    lv_obj_center(label1);

    lv_obj_t * btn_range2 = lv_button_create(main_obj);
    lv_obj_set_size(btn_range2, 100, 30);
    lv_obj_align_to(btn_range2, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_t * label2 = lv_label_create(btn_range2);
    lv_label_set_text(label2, "25-75");
    lv_obj_center(label2);

    lv_obj_t * btn_range3 = lv_button_create(main_obj);
    lv_obj_set_size(btn_range3, 100, 30);
    lv_obj_align_to(btn_range3, slider, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 10);
    lv_obj_t * label3 = lv_label_create(btn_range3);
    lv_label_set_text(label3, "Remove");
    lv_obj_center(label3);

    /* Add event callbacks */
    lv_obj_add_event_cb(btn_range1, set_range_0_50_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_range2, set_range_25_75_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_range3, remove_range_cb, LV_EVENT_CLICKED, NULL);
}

/*********************
 *   STATIC FUNCTIONS
 **********************/

static void set_range_0_50_cb(lv_event_t * e) 
{
    LV_UNUSED(e);
    if(dynamic_subject && dynamic_range_label) {
        lv_subject_set_int_range(dynamic_subject, 0, 50);
        lv_label_set_text(dynamic_range_label, "Range: 0-50");
    }
}

static void set_range_25_75_cb(lv_event_t * e) 
{
    LV_UNUSED(e);
    if(dynamic_subject && dynamic_range_label) {
        lv_subject_set_int_range(dynamic_subject, 25, 75);
        lv_label_set_text(dynamic_range_label, "Range: 25-75");
    }
}

static void remove_range_cb(lv_event_t * e) 
{
    LV_UNUSED(e);
    if(dynamic_subject && dynamic_range_label) {
        lv_subject_remove_int_range(dynamic_subject);
        lv_label_set_text(dynamic_range_label, "No range set");
    }
}

#endif /*LV_USE_OBSERVER && LV_BUILD_EXAMPLES*/ 