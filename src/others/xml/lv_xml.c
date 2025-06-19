/**
 * @file lv_xml.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_xml.h"
#if LV_USE_XML

#include "lv_xml_base_types.h"
#include "lv_xml_parser.h"
#include "lv_xml_component.h"
#include "lv_xml_component_private.h"
#include "lv_xml_widget.h"
#include "lv_xml_style.h"
#include "lv_xml.h"
#include "lv_xml_utils.h"
#include "lv_xml_private.h"
#include "parsers/lv_xml_obj_parser.h"
#include "parsers/lv_xml_button_parser.h"
#include "parsers/lv_xml_label_parser.h"
#include "parsers/lv_xml_image_parser.h"
#include "parsers/lv_xml_bar_parser.h"
#include "parsers/lv_xml_slider_parser.h"
#include "parsers/lv_xml_tabview_parser.h"
#include "parsers/lv_xml_chart_parser.h"
#include "parsers/lv_xml_table_parser.h"
#include "parsers/lv_xml_dropdown_parser.h"
#include "parsers/lv_xml_roller_parser.h"
#include "parsers/lv_xml_scale_parser.h"
#include "parsers/lv_xml_buttonmatrix_parser.h"
#include "parsers/lv_xml_spangroup_parser.h"
#include "parsers/lv_xml_textarea_parser.h"
#include "parsers/lv_xml_keyboard_parser.h"
#include "parsers/lv_xml_arc_parser.h"
#include "parsers/lv_xml_checkbox_parser.h"
#include "parsers/lv_xml_canvas_parser.h"
#include "parsers/lv_xml_calendar_parser.h"
#include "../../libs/expat/expat.h"
#include "../../draw/lv_draw_image.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void view_start_element_handler(void * user_data, const char * name, const char ** attrs);
static void view_end_element_handler(void * user_data, const char * name);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_xml_init(void)
{
    lv_xml_component_init();

    lv_xml_register_font(NULL, "lv_font_default", lv_font_get_default());

    lv_xml_widget_register("lv_obj", lv_xml_obj_create, lv_xml_obj_apply);
    lv_xml_widget_register("lv_button", lv_xml_button_create, lv_xml_button_apply);
    lv_xml_widget_register("lv_label", lv_xml_label_create, lv_xml_label_apply);
    lv_xml_widget_register("lv_image", lv_xml_image_create, lv_xml_image_apply);
    lv_xml_widget_register("lv_bar", lv_xml_bar_create, lv_xml_bar_apply);
    lv_xml_widget_register("lv_slider", lv_xml_slider_create, lv_xml_slider_apply);
    lv_xml_widget_register("lv_tabview", lv_xml_tabview_create, lv_xml_tabview_apply);
    lv_xml_widget_register("lv_tabview-tab_bar", lv_xml_tabview_tab_bar_create, lv_xml_tabview_tab_bar_apply);
    lv_xml_widget_register("lv_tabview-tab", lv_xml_tabview_tab_create, lv_xml_tabview_tab_apply);
    lv_xml_widget_register("lv_chart", lv_xml_chart_create, lv_xml_chart_apply);
    lv_xml_widget_register("lv_chart-cursor", lv_xml_chart_cursor_create, lv_xml_chart_cursor_apply);
    lv_xml_widget_register("lv_chart-series", lv_xml_chart_series_create, lv_xml_chart_series_apply);
    lv_xml_widget_register("lv_chart-axis", lv_xml_chart_axis_create, lv_xml_chart_axis_apply);
    lv_xml_widget_register("lv_table", lv_xml_table_create, lv_xml_table_apply);
    lv_xml_widget_register("lv_table-column", lv_xml_table_column_create, lv_xml_table_column_apply);
    lv_xml_widget_register("lv_table-cell", lv_xml_table_cell_create, lv_xml_table_cell_apply);
    lv_xml_widget_register("lv_dropdown", lv_xml_dropdown_create, lv_xml_dropdown_apply);
    lv_xml_widget_register("lv_dropdown-list", lv_xml_dropdown_list_create, lv_xml_dropdown_list_apply);
    lv_xml_widget_register("lv_roller", lv_xml_roller_create, lv_xml_roller_apply);
    lv_xml_widget_register("lv_scale", lv_xml_scale_create, lv_xml_scale_apply);
    lv_xml_widget_register("lv_scale-section", lv_xml_scale_section_create, lv_xml_scale_section_apply);
    lv_xml_widget_register("lv_spangroup", lv_xml_spangroup_create, lv_xml_spangroup_apply);
    lv_xml_widget_register("lv_spangroup-span", lv_xml_spangroup_span_create, lv_xml_spangroup_span_apply);
    lv_xml_widget_register("lv_buttonmatrix", lv_xml_buttonmatrix_create, lv_xml_buttonmatrix_apply);
    lv_xml_widget_register("lv_textarea", lv_xml_textarea_create, lv_xml_textarea_apply);
    lv_xml_widget_register("lv_keyboard", lv_xml_keyboard_create, lv_xml_keyboard_apply);
    lv_xml_widget_register("lv_arc", lv_xml_arc_create, lv_xml_arc_apply);
    lv_xml_widget_register("lv_checkbox", lv_xml_checkbox_create, lv_xml_checkbox_apply);
    lv_xml_widget_register("lv_canvas", lv_xml_canvas_create, lv_xml_canvas_apply);
    lv_xml_widget_register("lv_calendar", lv_xml_calendar_create, lv_xml_calendar_apply);
    lv_xml_widget_register("lv_calendar-header_arrow", lv_xml_calendar_header_arrow_create,
                           lv_xml_calendar_header_arrow_apply);
    lv_xml_widget_register("lv_calendar-header_dropdown", lv_xml_calendar_header_dropdown_create,
                           lv_xml_calendar_header_dropdown_apply);

    lv_xml_widget_register("lv_obj-style", lv_obj_xml_style_create, lv_obj_xml_style_apply);
    lv_xml_widget_register("lv_obj-remove_style", lv_obj_xml_remove_style_create, lv_obj_xml_remove_style_apply);
    lv_xml_widget_register("lv_obj-remove_style_all", lv_obj_xml_remove_style_all_create,
                           lv_obj_xml_remove_style_all_apply);

    lv_xml_widget_register("lv_obj-event_cb", lv_obj_xml_event_cb_create, lv_obj_xml_event_cb_apply);
    lv_xml_widget_register("lv_obj-subject_set_int", lv_obj_xml_subject_set_create, lv_obj_xml_subject_set_apply);
    lv_xml_widget_register("lv_obj-subject_set_string", lv_obj_xml_subject_set_create, lv_obj_xml_subject_set_apply);
    lv_xml_widget_register("lv_obj-subject_increment", lv_obj_xml_subject_increment_create,
                           lv_obj_xml_subject_increment_apply);
    lv_xml_widget_register("lv_obj-screen_load_event", lv_obj_xml_screen_load_event_create,
                           lv_obj_xml_screen_load_event_apply);
    lv_xml_widget_register("lv_obj-screen_create_event", lv_obj_xml_screen_create_event_create,
                           lv_obj_xml_screen_create_event_apply);

    lv_xml_widget_register("lv_obj-bind_style", lv_obj_xml_bind_style_create, lv_obj_xml_bind_style_apply);
    lv_xml_widget_register("lv_obj-bind_flag_if_eq", lv_obj_xml_bind_flag_create, lv_obj_xml_bind_flag_apply);
    lv_xml_widget_register("lv_obj-bind_flag_if_not_eq", lv_obj_xml_bind_flag_create, lv_obj_xml_bind_flag_apply);
    lv_xml_widget_register("lv_obj-bind_flag_if_gt", lv_obj_xml_bind_flag_create, lv_obj_xml_bind_flag_apply);
    lv_xml_widget_register("lv_obj-bind_flag_if_lt", lv_obj_xml_bind_flag_create, lv_obj_xml_bind_flag_apply);
    lv_xml_widget_register("lv_obj-bind_flag_if_ge", lv_obj_xml_bind_flag_create, lv_obj_xml_bind_flag_apply);
    lv_xml_widget_register("lv_obj-bind_flag_if_le", lv_obj_xml_bind_flag_create, lv_obj_xml_bind_flag_apply);

    lv_xml_widget_register("lv_obj-bind_state_if_eq", lv_obj_xml_bind_state_create, lv_obj_xml_bind_state_apply);
    lv_xml_widget_register("lv_obj-bind_state_if_not_eq", lv_obj_xml_bind_state_create, lv_obj_xml_bind_state_apply);
    lv_xml_widget_register("lv_obj-bind_state_if_gt", lv_obj_xml_bind_state_create, lv_obj_xml_bind_state_apply);
    lv_xml_widget_register("lv_obj-bind_state_if_lt", lv_obj_xml_bind_state_create, lv_obj_xml_bind_state_apply);
    lv_xml_widget_register("lv_obj-bind_state_if_ge", lv_obj_xml_bind_state_create, lv_obj_xml_bind_state_apply);
    lv_xml_widget_register("lv_obj-bind_state_if_le", lv_obj_xml_bind_state_create, lv_obj_xml_bind_state_apply);
}

void * lv_xml_create_in_scope(lv_obj_t * parent, lv_xml_component_scope_t * parent_scope,
                              lv_xml_component_scope_t * scope,
                              const char ** attrs)
{
    /* Initialize the parser state */
    lv_xml_parser_state_t state;
    lv_xml_parser_state_init(&state);
    state.scope = *scope; /*Scope won't be modified here, so it's safe to copy it by value*/
    state.parent = parent;
    state.parent_attrs = attrs;
    state.parent_scope = parent_scope;

    lv_obj_t ** parent_node = lv_ll_ins_head(&state.parent_ll);
    *parent_node = parent;

    /* Create an XML parser and set handlers */
    XML_Memory_Handling_Suite mem_handlers;
    mem_handlers.malloc_fcn = lv_malloc;
    mem_handlers.realloc_fcn = lv_realloc;
    mem_handlers.free_fcn = lv_free;
    XML_Parser parser = XML_ParserCreate_MM(NULL, &mem_handlers, NULL);
    XML_SetUserData(parser, &state);
    XML_SetElementHandler(parser, view_start_element_handler, view_end_element_handler);

    /* Parse the XML */
    if(XML_Parse(parser, scope->view_def, lv_strlen(scope->view_def), XML_TRUE) == XML_STATUS_ERROR) {
        LV_LOG_WARN("XML parsing error: %s on line %lu", XML_ErrorString(XML_GetErrorCode(parser)),
                    XML_GetCurrentLineNumber(parser));
        XML_ParserFree(parser);
        return NULL;
    }

    state.item = state.view;

#if LV_USE_OBJ_NAME
    /*Set a default indexed name*/
    if(state.item && lv_obj_get_name(state.item) == NULL) {
        char name_buf[128];
        lv_snprintf(name_buf, sizeof(name_buf), "%s_#", scope->name);
        lv_obj_set_name(state.item, name_buf);
    }
#endif

    lv_ll_clear(&state.parent_ll);
    XML_ParserFree(parser);

    return state.view;
}

void * lv_xml_create(lv_obj_t * parent, const char * name, const char ** attrs)
{
    lv_obj_t * item = NULL;

    /* Select the widget specific parser type based on the name */
    lv_widget_processor_t * p = lv_xml_widget_get_processor(name);
    if(p) {
        lv_xml_parser_state_t state;
        lv_xml_parser_state_init(&state);
        state.parent = parent;

        /* When a component is just created there is no scope where
         * its styles, constants, etc are stored.
         * So leave state.scope = NULL which means the global context.*/

        state.item = p->create_cb(&state, attrs);
        if(attrs) {
            p->apply_cb(&state, attrs);
        }
        return state.item;
    }

    lv_xml_component_scope_t * scope = lv_xml_component_get_scope(name);
    if(scope) {
        item = lv_xml_create_in_scope(parent, NULL, scope, attrs);

        if(attrs) {
            lv_xml_parser_state_t state;
            lv_xml_parser_state_init(&state);
            state.parent = parent;
            state.item = item;

            /* When a component is just created there is no scope where
             * its styles, constants, etc are stored.
             * So leave state.scope = NULL which means the global context.*/

            p = lv_xml_widget_get_extended_widget_processor(scope->extends);
            p->apply_cb(&state, attrs);
        }

        return item;
    }

    /* If it isn't a component either then it is unknown */
    LV_LOG_WARN("'%s' is not a known widget, element, or component", name);
    return NULL;
}

lv_result_t lv_xml_register_font(lv_xml_component_scope_t * scope, const char * name, const lv_font_t * font)
{

    if(scope == NULL) scope = lv_xml_component_get_scope("globals");
    if(scope == NULL) {
        LV_LOG_WARN("No component found to register font `%s`", name);
        return LV_RESULT_INVALID;
    }

    lv_xml_font_t * f;
    LV_LL_READ(&scope->font_ll, f) {
        if(lv_streq(f->name, name)) {
            LV_LOG_INFO("Font %s is already registered. Don't register it again.", name);
            return LV_RESULT_OK;
        }
    }

    f = lv_ll_ins_head(&scope->font_ll);
    lv_memzero(f, sizeof(*f));
    f->name = lv_strdup(name);
    f->font = font;

    return LV_RESULT_OK;
}

const lv_font_t * lv_xml_get_font(lv_xml_component_scope_t * scope, const char * name)
{
    lv_xml_font_t * f;
    if(scope) {
        LV_LL_READ(&scope->font_ll, f) {
            if(lv_streq(f->name, name)) return f->font;
        }
    }

    /*If not found in the component check the global space*/
    if((scope == NULL || scope->name == NULL) || !lv_streq(scope->name, "globals")) {
        scope = lv_xml_component_get_scope("globals");
        if(scope) {
            LV_LL_READ(&scope->font_ll, f) {
                if(lv_streq(f->name, name)) return f->font;
            }
        }
    }

    LV_LOG_WARN("No font was found with name \"%s\". Using LV_FONT_DEFAULT instead.", name);
    return lv_font_get_default();
}

lv_result_t lv_xml_register_subject(lv_xml_component_scope_t * scope, const char * name, lv_subject_t * subject)
{
    if(scope == NULL) scope = lv_xml_component_get_scope("globals");
    if(scope == NULL) {
        LV_LOG_WARN("No component found to register subject `%s`", name);
        return LV_RESULT_INVALID;
    }


    lv_xml_subject_t * s;
    LV_LL_READ(&scope->subjects_ll, s) {
        if(lv_streq(s->name, name)) {
            LV_LOG_INFO("Subject %s is already registered. Don't register it again.", name);
            return LV_RESULT_OK;
        }
    }

    s = lv_ll_ins_head(&scope->subjects_ll);
    lv_memzero(s, sizeof(*s));
    s->name = lv_strdup(name);
    s->subject = subject;

    return LV_RESULT_OK;
}

lv_subject_t * lv_xml_get_subject(lv_xml_component_scope_t * scope, const char * name)
{
    lv_xml_subject_t * s;
    if(scope) {
        LV_LL_READ(&scope->subjects_ll, s) {
            if(lv_streq(s->name, name)) return s->subject;
        }
    }

    /*If not found in the component check the global space*/
    if((scope == NULL || scope->name == NULL) || !lv_streq(scope->name, "globals")) {
        scope = lv_xml_component_get_scope("globals");
        if(scope) {
            LV_LL_READ(&scope->subjects_ll, s) {
                if(lv_streq(s->name, name)) return s->subject;
            }
        }
    }

    LV_LOG_WARN("No subject was found with name \"%s\".", name);
    return NULL;
}

lv_result_t lv_xml_register_const(lv_xml_component_scope_t * scope, const char * name, const char * value)
{
    if(scope == NULL) scope = lv_xml_component_get_scope("globals");
    if(scope == NULL) {
        LV_LOG_WARN("No component found to register constant `%s`", name);
        return LV_RESULT_INVALID;
    }

    lv_xml_const_t * cnst;
    LV_LL_READ(&scope->const_ll, cnst) {
        if(lv_streq(cnst->name, name)) {
            LV_LOG_INFO("Const %s is already registered. Don't register it again.", name);
            return LV_RESULT_OK;
        }
    }

    cnst = lv_ll_ins_head(&scope->const_ll);
    lv_memzero(cnst, sizeof(*cnst));

    cnst->name = lv_strdup(name);
    cnst->value = lv_strdup(value);

    return LV_RESULT_OK;
}

const char * lv_xml_get_const(lv_xml_component_scope_t * scope, const char * name)
{

    if(scope == NULL) scope = lv_xml_component_get_scope("globals");
    if(scope == NULL) return NULL;

    lv_xml_const_t * cnst;
    if(scope) {
        LV_LL_READ(&scope->const_ll, cnst) {
            if(lv_streq(cnst->name, name)) return cnst->value;
        }
    }

    /*If not found in the component check the global space*/
    if((scope == NULL || scope->name == NULL) || !lv_streq(scope->name, "globals")) {
        scope = lv_xml_component_get_scope("globals");
        if(scope) {
            LV_LL_READ(&scope->const_ll, cnst) {
                if(lv_streq(cnst->name, name)) return cnst->value;
            }
        }
    }

    LV_LOG_WARN("No constant was found with name \"%s\".", name);
    return NULL;
}


lv_result_t lv_xml_register_image(lv_xml_component_scope_t * scope, const char * name, const void * src)
{
    if(scope == NULL) scope = lv_xml_component_get_scope("globals");
    if(scope == NULL) {
        LV_LOG_WARN("No component found to register image `%s`", name);
        return LV_RESULT_INVALID;
    }

    lv_xml_image_t * img;
    LV_LL_READ(&scope->image_ll, img) {
        if(lv_streq(img->name, name)) {
            LV_LOG_INFO("Image %s is already registered. Don't register it again.", name);
            return LV_RESULT_OK;
        }
    }

    img = lv_ll_ins_head(&scope->image_ll);
    lv_memzero(img, sizeof(*img));
    img->name = lv_strdup(name);
    if(lv_image_src_get_type(src) == LV_IMAGE_SRC_FILE) {
        img->src = lv_strdup(src);
    }
    else {
        img->src = src;
    }

    return LV_RESULT_OK;
}

const void * lv_xml_get_image(lv_xml_component_scope_t * scope, const char * name)
{
    if(scope == NULL) scope = lv_xml_component_get_scope("globals");
    if(scope == NULL) return NULL;

    lv_xml_image_t * img;
    if(scope) {
        LV_LL_READ(&scope->image_ll, img) {
            if(lv_streq(img->name, name)) return img->src;
        }
    }

    /*If not found in the component check the global space*/
    if((scope == NULL || scope->name == NULL) || !lv_streq(scope->name, "globals")) {
        scope = lv_xml_component_get_scope("globals");
        if(scope) {
            LV_LL_READ(&scope->image_ll, img) {
                if(lv_streq(img->name, name)) return img->src;
            }
        }
    }

    LV_LOG_WARN("No image was found with name \"%s\"", name);
    return NULL;
}

lv_result_t lv_xml_register_event_cb(lv_xml_component_scope_t * scope, const char * name, lv_event_cb_t cb)
{
    if(scope == NULL) scope = lv_xml_component_get_scope("globals");
    if(scope == NULL) {
        LV_LOG_WARN("No component found to register event `%s`", name);
        return LV_RESULT_INVALID;
    }

    lv_xml_event_cb_t * e;
    LV_LL_READ(&scope->event_ll, e) {
        if(lv_streq(e->name, name)) {
            LV_LOG_INFO("Event_cb %s is already registered. Don't register it again.", name);
            return LV_RESULT_OK;
        }
    }

    e = lv_ll_ins_head(&scope->event_ll);
    lv_memzero(e, sizeof(*e));
    e->name = lv_strdup(name);
    e->cb = cb;

    return LV_RESULT_OK;
}


lv_event_cb_t lv_xml_get_event_cb(lv_xml_component_scope_t * scope, const char * name)
{
    if(scope == NULL) scope = lv_xml_component_get_scope("globals");
    if(scope == NULL) return NULL;

    lv_xml_event_cb_t * e;
    if(scope) {
        LV_LL_READ(&scope->event_ll, e) {
            if(lv_streq(e->name, name)) return e->cb;
        }
    }

    /*If not found in the component check the global space*/
    if((scope == NULL || scope->name == NULL) || !lv_streq(scope->name, "globals")) {
        scope = lv_xml_component_get_scope("globals");
        if(scope) {
            LV_LL_READ(&scope->event_ll, e) {
                if(lv_streq(e->name, name)) return e->cb;
            }
        }
    }

    LV_LOG_WARN("No event was found with name \"%s\"", name);
    return NULL;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static const char * get_param_type(lv_xml_component_scope_t * scope, const char * name)
{
    lv_xml_param_t * p;
    LV_LL_READ(&scope->param_ll, p) {
        if(lv_streq(p->name, name)) return p->type;
    }
    return NULL;
}

static const char * get_param_default(lv_xml_component_scope_t * scope, const char * name)
{
    lv_xml_param_t * p;
    LV_LL_READ(&scope->param_ll, p) {
        if(lv_streq(p->name, name)) return p->def;
    }
    return NULL;
}

static const char ** resolve_params(lv_xml_component_scope_t * item_scope, lv_xml_component_scope_t * parent_scope,
                                    const char ** item_attrs, const char ** parent_attrs)
{
    /* Count the number of attributes */
    uint32_t attr_count = 0;
    while(item_attrs[attr_count]) attr_count += 2;

    /* Create a copy of the attributes array */
    const char ** new_attrs = lv_malloc((attr_count + 1) * sizeof(char *));
    if(new_attrs == NULL) return NULL;

    /* Copy all attributes and process parameter substitution */
    uint32_t i;
    for(i = 0; i < attr_count; i += 2) {
        const char * name = item_attrs[i];
        const char * value = item_attrs[i + 1];

        /* Copy the attribute name */
        new_attrs[i] = name;

        if(lv_streq(name, "styles")) {
            /* Styles will handle parameter substitution themselves */
            new_attrs[i + 1] = value;
            continue;
        }

        /* Check if the value contains parameter substitution */
        if(lv_strchr(value, '$') != NULL) {
            /* Calculate the maximum possible length for the result string */
            size_t input_len = lv_strlen(value);
            size_t max_result_len = input_len * 4; /* Conservative estimate */
            char * result = lv_malloc(max_result_len);
            if(result == NULL) {
                new_attrs[i + 1] = "";
                continue;
            }

            result[0] = '\0';
            const char * current = value;

            while(*current) {
                const char * param_start = lv_strchr(current, '$');
                if(param_start == NULL) {
                    /* No more parameters, append the rest of the string */
                    if(lv_strlen(result) + lv_strlen(current) < max_result_len) {
                        lv_strcat(result, current);
                    }
                    break;
                }

                /* Append text before the parameter */
                size_t prefix_len = param_start - current;
                if(prefix_len > 0) {
                    char * prefix = lv_malloc(prefix_len + 1);
                    if(prefix == NULL) {
                        lv_free(result);
                        new_attrs[i + 1] = "";
                        goto next_attr;
                    }
                    lv_memcpy(prefix, current, prefix_len);
                    prefix[prefix_len] = '\0';
                    if(lv_strlen(result) + prefix_len < max_result_len) {
                        lv_strcat(result, prefix);
                    }
                    lv_free(prefix);
                }

                /* Extract parameter name */
                const char * param_name_start = param_start + 1;
                const char * param_name_end = param_name_start;

                /* Find the end of the parameter name (alphanumeric + underscore) */
                while(*param_name_end && ((*param_name_end >= 'a' && *param_name_end <= 'z') ||
                                          (*param_name_end >= 'A' && *param_name_end <= 'Z') ||
                                          (*param_name_end >= '0' && *param_name_end <= '9') ||
                                          *param_name_end == '_')) {
                    param_name_end++;
                }

                if(param_name_end > param_name_start) {
                    /* Extract parameter name */
                    size_t param_name_len = param_name_end - param_name_start;
                    char * param_name = lv_malloc(param_name_len + 1);
                    if(param_name == NULL) {
                        lv_free(result);
                        new_attrs[i + 1] = "";
                        goto next_attr;
                    }
                    lv_memcpy(param_name, param_name_start, param_name_len);
                    param_name[param_name_len] = '\0';

                    /* Get parameter type and value */
                    const char * type = get_param_type(item_scope, param_name);
                    if(type == NULL) {
                        LV_LOG_WARN("'%s' parameter is not defined on '%s'", param_name, item_scope->name);
                        if(lv_strlen(result) + 1 + lv_strlen(param_name) < max_result_len) {
                            lv_strcat(result, "$");
                            lv_strcat(result, param_name);
                        }
                    }
                    else {
                        const char * ext_value = lv_xml_get_value_of(parent_attrs, param_name);
                        if(ext_value) {
                            /* If the value is not resolved earlier, use the default value */
                            if(ext_value[0] == '#' || ext_value[0] == '$') {
                                ext_value = get_param_default(item_scope, param_name);
                            }
                            else if(lv_streq(type, "style")) {
                                lv_xml_style_t * s = lv_xml_get_style_by_name(parent_scope, ext_value);
                                if(s) ext_value = s->long_name;
                            }
                        }
                        else {
                            /* If the API attribute is not provided, use the default value */
                            ext_value = get_param_default(item_scope, param_name);
                        }

                        if(ext_value) {
                            if(lv_strlen(result) + lv_strlen(ext_value) < max_result_len) {
                                lv_strcat(result, ext_value);
                            }
                        }
                        else {
                            /* No value found, keep the parameter as-is */
                            if(lv_strlen(result) + 1 + lv_strlen(param_name) < max_result_len) {
                                lv_strcat(result, "$");
                                lv_strcat(result, param_name);
                            }
                        }
                    }

                    lv_free(param_name);
                }
                else {
                    /* Invalid parameter name, keep the $ as-is */
                    if(lv_strlen(result) + 1 < max_result_len) {
                        lv_strcat(result, "$");
                    }
                }

                current = param_name_end;
            }

            new_attrs[i + 1] = result;
        }
        else {
            /* No parameter substitution needed, copy the original value */
            new_attrs[i + 1] = value;
        }
next_attr:
        ;
    }

    /* Null-terminate the array */
    new_attrs[attr_count] = NULL;

    return new_attrs;
}

static void free_resolved_attrs(const char ** attrs, const char ** original_attrs)
{
    if(attrs == NULL) return;

    /* Free any allocated strings (parameter substitution results) */
    uint32_t i = 0;
    while(attrs[i]) {
        /* Free values that were allocated by parameter substitution
         * (values that are different from the original) */
        if(attrs[i + 1] != original_attrs[i + 1]) {
            lv_free((void *)attrs[i + 1]);
        }
        i += 2; /* Skip to next attribute pair */
    }

    /* Free the attributes array itself */
    lv_free(attrs);
}

static void resolve_consts(const char ** item_attrs, lv_xml_component_scope_t * scope)
{
    uint32_t i;
    for(i = 0; item_attrs[i]; i += 2) {
        const char * name = item_attrs[i];
        const char * value = item_attrs[i + 1];
        if(lv_streq(name, "styles")) continue; /*Styles will handle it themselves*/
        if(value[0] == '#') {
            const char * value_clean = &value[1];

            const char * const_value = lv_xml_get_const(scope, value_clean);
            if(const_value) {
                item_attrs[i + 1] = const_value;
            }
            /*If the const attribute is not provide don't set it*/
            else {
                item_attrs[i] = "";
                item_attrs[i + 1] = "";
            }
        }
    }
}

static void view_start_element_handler(void * user_data, const char * name, const char ** attrs)
{
    lv_xml_parser_state_t * state = (lv_xml_parser_state_t *)user_data;
    state->tag_name = name;
    bool is_view = false;

    if(lv_streq(name, "view")) {
        const char * extends = lv_xml_get_value_of(attrs, "extends");
        name = extends ? extends : "lv_obj";
        is_view = true;
    }

    lv_obj_t ** current_parent_p = lv_ll_get_tail(&state->parent_ll);
    if(current_parent_p == NULL) {
        if(state->parent == NULL) {
            LV_LOG_ERROR("There is no parent object available for %s. This also should never happen.", name);
            return;
        }
        else {
            current_parent_p = &state->parent;
        }
    }
    else {
        state->parent = *current_parent_p;
    }

    /*In `state->attrs` we have parameters of the component creation
     *E.g. <my_button x="10" title="Hello"/>
     *In `attrs` we have the attributes of child of the view.
     *E.g. in `my_button` `<lv_label x="5" text="${title}".
     *This function creates a new attributes array with parameter substitution
     *E.g. "text", "${title}" -> "text", "Hello" */
    const char ** original_attrs = attrs;
    const char ** resolved_attrs = resolve_params(&state->scope, state->parent_scope, attrs, state->parent_attrs);
    bool attrs_were_resolved = false;

    if(resolved_attrs == NULL) {
        /* Memory allocation failed, use original attrs */
        resolved_attrs = attrs;
    }
    else if(resolved_attrs != attrs) {
        attrs_were_resolved = true;
    }

    resolve_consts(resolved_attrs, &state->scope);

    /* Use resolved_attrs for the rest of the processing */
    attrs = resolved_attrs;

    void * item = NULL;
    /* Select the widget specific parser type based on the name */
    lv_widget_processor_t * p = lv_xml_widget_get_processor(name);
    if(p) {
        item = p->create_cb(state, attrs);
        state->item = item;


        /*If it's a widget remove all styles. E.g. if it extends an `lv_button`
         *now it has the button theme styles. However if it were a real widget
         *it had e.g. `my_widget_class` so the button's theme wouldn't apply on it.
         *Removing the style will ensure a better preview*/
        if(state->scope.is_widget && is_view) lv_obj_remove_style_all(item);

        /*Apply the attributes from e.g. `<lv_slider value="30" x="20">`*/
        if(item) {
            p->apply_cb(state, attrs);
        }
    }

    /* If not a widget, check if it is a component */
    if(item == NULL) {
        item = lv_xml_component_process(state, name, attrs);
        state->item = item;
    }

    /* If it isn't a component either then it is unknown */
    if(item == NULL) {
        LV_LOG_WARN("'%s' is not a known widget, element, or component", name);
        /* Clean up resolved attributes before returning */
        if(attrs_were_resolved) {
            free_resolved_attrs(resolved_attrs, original_attrs);
        }
        return;
    }

    void ** new_parent = lv_ll_ins_tail(&state->parent_ll);
    *new_parent = item;

    if(is_view) {
        state->view = item;
    }

    /* Clean up resolved attributes */
    if(attrs_were_resolved) {
        free_resolved_attrs(resolved_attrs, original_attrs);
    }
}

static void view_end_element_handler(void * user_data, const char * name)
{
    LV_UNUSED(name);

    lv_xml_parser_state_t * state = (lv_xml_parser_state_t *)user_data;

    lv_obj_t ** current_parent = lv_ll_get_tail(&state->parent_ll);
    if(current_parent) {
        lv_ll_remove(&state->parent_ll, current_parent);
        lv_free(current_parent);
    }
}


#endif /* LV_USE_XML */
