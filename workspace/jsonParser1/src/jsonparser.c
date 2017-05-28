#include <tizen.h>
#include "jsonparser.h"
#include <dlog.h>
#include<json-glib/json-glib.h>
#include <stdio.h>
#include <pthread.h>


static const gchar *test_nested_objects[] = {
  "{ \"array\" : [ false, \"foo\" ], \"object\" : { \"foo\" : true } }",
  "{ "
    "\"type\" : \"ClutterGroup\", "
    "\"width\" : 1, "
    "\"children\" : [ "
      "{ "
        "\"type\" : \"ClutterRectangle\", "
        "\"children\" : [ "
          "{ \"type\" : \"ClutterText\", \"text\" : \"hello there\" }"
        "] "
      "}, "
      "{ "
        "\"type\" : \"ClutterGroup\", "
        "\"width\" : 1, "
        "\"children\" : [ "
          "{ \"type\" : \"ClutterText\", \"text\" : \"hello\" }"
        "] "
      "} "
    "] "
  "}"
};

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *layout;
	Evas_Object *label;
	Evas_Object *parseButton1;
	Evas_Object *parseButton2;
	Evas_Object *nf;
	Evas_Object *nf2;
} appdata_s;


static void
win_delete_request_cb(void *data , Evas_Object *obj , void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

gchar*
ExtractValue(JsonNode *node)
{
    gchar * value;
    GType valueType=json_node_get_value_type(node);
	switch(valueType)
    {
        case G_TYPE_STRING:
        value = json_node_dup_string(node);
        break;

        case G_TYPE_INTEGER:
        value= malloc(20*sizeof(gchar));
        sprintf (value, "%d", (int)json_node_get_int(node));
        break;

        case G_TYPE_DOUBLE:
        value= malloc(40*sizeof(gchar));
        sprintf (value, "%f", json_node_get_double(node));
        break;

        case G_TYPE_BOOLEAN:
        value= malloc(6*sizeof(gchar));
        if(json_node_get_boolean(node)==TRUE)
        {
        	sprintf (value, "%s", "true");
        }
        else
        {
        	sprintf (value, "%s", "false");
        }
        break;

        default:
        value= malloc(22*sizeof(gchar));
        sprintf (value, "%s", "Value of unknown type");
        break;
    }
	return value;
}


Eina_List*
ParseJsonEntity(JsonNode *root, bool isArrayParsing)
{
    Eina_List *jsonList = NULL;
    if (JSON_NODE_TYPE (root) == JSON_NODE_ARRAY)
    {
         JsonArray* array = json_node_get_array(root);
         guint arraySize = json_array_get_length (array);
         JsonNode *arrayElem;
         for(guint i=0;i<arraySize;i++)
	     {
            Eina_List *jsonArrElemList=NULL;
        	arrayElem = json_array_get_element(array,i);
        	jsonArrElemList=ParseJsonEntity(arrayElem, true);
        	jsonList= eina_list_merge(jsonList,jsonArrElemList);
        	//add array member (with pair key/value) to the end of the list
	     }
    }
    else if (JSON_NODE_TYPE (root) == JSON_NODE_VALUE)
    {

        jsonList = eina_list_append(jsonList,ExtractValue(root));
     	if(isArrayParsing)
     	{
           jsonList = eina_list_append(jsonList,NULL);
           //add member of list with data=NULL (for arrays without keys as separator)
     	}
    }
    else if(JSON_NODE_TYPE (root) == JSON_NODE_OBJECT)
    {
        JsonObject *object = NULL;

        object = json_node_get_object (root);

        if (object != NULL)
        {
            GList * keysList = NULL;
            GList * valList = NULL;
            guint size;

            size = json_object_get_size (object);

            keysList = json_object_get_members (object);
            valList = json_object_get_values (object);

		    JsonNode *tmpNode;
		    gchar *keyName;

 	        for(int j=0;j<size;j++)
	        {
 	 		    if (keysList)
 	 		    {
 	                keyName = malloc( (strlen(keysList->data) +1)*sizeof(gchar) );
 		            sprintf (keyName, "%s", (gchar*)(keysList->data));
 	 	            jsonList = eina_list_append(jsonList,keyName);
 	 		    }
 		        if (valList)
 		        {
 		        	tmpNode=(JsonNode*)(valList->data);
 		        }

 		        Eina_List *l= ParseJsonEntity(tmpNode,false);
 	            jsonList = eina_list_append(jsonList,l);

 	            keysList=g_list_next(keysList);
 		        valList=g_list_next(valList);

 	        }
    	    if (keysList != NULL) g_list_free(keysList);
    	    if (valList != NULL) g_list_free(valList);
        }
    }
    return jsonList;
}

void
printParsedList(Eina_List * jsonlist, int level)
{
    Eina_List *l;
    void *list_data;
    int i=0;
    EINA_LIST_FOREACH(jsonlist, l, list_data);
    {
        if(i%2){
        	int level_=level+1;
        	if(list_data!=NULL)
        	{
               printParsedList(list_data,level_);
            }
        }
        else
        {
           gchar* spaceOffset=NULL;
      	   spaceOffset = malloc((2*level+1)*sizeof(gchar));
           for(int i=0; i<level;i++){
        	   spaceOffset[2*i]='-';
        	   spaceOffset[2*i+1]='>';
           }
           spaceOffset[2*level]='\0';
           LOGI("%s %s" , spaceOffset, (gchar*)list_data);
           if(spaceOffset!=NULL) free(spaceOffset);
           if(list_data!=NULL) free(list_data);
           //for all keys and values added into Eina_List memory should be freed manually
        }
        i++;
    }
    eina_list_free(jsonlist);
}

static void
btn_parseArray_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	JsonParser *jsonParser;
	jsonParser = json_parser_new ();
    guint n_test_nested_objects = G_N_ELEMENTS (test_nested_objects);
    LOGI("ParseNestedObjects size %d",n_test_nested_objects);
    for (int i = 0; i < n_test_nested_objects; i++)
    {
        GError *error = NULL;

        if (!json_parser_load_from_data (jsonParser, test_nested_objects[i], -1, &error))
        {
            LOGI("ParseNestedObjects Error");
            g_error_free (error);
        }
        else
	    {
            int level =1;
            Eina_List *jsonList = NULL;
            jsonList =ParseJsonEntity(json_parser_get_root (jsonParser),false);
        	printParsedList(jsonList,level);
        }
    }
	g_object_unref(jsonParser);
}

static void
btn_parseFile_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	JsonParser *jsonParser = NULL;
	GError *error;
	error = NULL;

	const gchar *sharedResFolderPath = app_get_shared_resource_path();
	const gchar *filename ="jsonSample.json";

	gchar *filePath =NULL;

    if(sharedResFolderPath != NULL)
    {
    	size_t size= strlen(sharedResFolderPath) +strlen(filename)+1;
    	filePath = malloc(size*sizeof(gchar));
    	eina_strlcpy(filePath, sharedResFolderPath ,strlen(sharedResFolderPath)+1);
        eina_strlcat(filePath, filename ,size);
        LOGI("Json file path is %s", filePath);
    }
    else
    {
    	LOGE("No file to load");
        return;
    }

	jsonParser = json_parser_new ();

	if(jsonParser)
	{
	    json_parser_load_from_file (jsonParser, filePath, &error);
	}
	else
	{
		LOGE("Unable to create parser");
	}

	if (error)
	{
		LOGE("Unable to load json file %s",filePath);
		LOGE("Error->code=%d", error->code);
		LOGE("Error->message=%s", error->message);
	}
	else
	{
        JsonNode *root;
        root =json_parser_get_root (jsonParser);
        if (NULL != root)
        {
            int level =1;
            Eina_List *jsonList = NULL;
        	jsonList = ParseJsonEntity(root,false);
        	printParsedList(jsonList,level);
        }
	}

	g_object_unref(jsonParser);
	if(filePath) free(filePath);
}

static void
create_base_gui(appdata_s *ad)
{
	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

//	/* Conformant */
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

//	/* Base Layout */
	ad->layout = elm_layout_add(ad->win);
	evas_object_size_hint_weight_set(ad->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(ad->layout, "layout", "application", "default");
	elm_win_resize_object_add(ad->win, ad->layout);
	evas_object_show(ad->layout);

	elm_object_content_set(ad->win, ad->layout);
	evas_object_show(ad->win);

	ad->nf = elm_naviframe_add(ad->layout);

	//-------------------------------MOVE TO PROCEDURE

    Evas_Object *scroller;

    scroller= elm_scroller_add(ad->nf);
    elm_scroller_bounce_set(scroller, EINA_FALSE, EINA_TRUE);
    elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
    evas_object_show(scroller);


	//----------
	Evas_Object *box;

	box = elm_box_add(scroller);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_padding_set(box, 0, 5 * elm_config_scale_get());
	evas_object_show(box);

	ad->parseButton1 = elm_button_add(box);
	elm_object_text_set(ad->parseButton1, "Parse from File");
	evas_object_size_hint_min_set(ad->parseButton1, ELM_SCALE_SIZE(140), ELM_SCALE_SIZE(40));
	evas_object_smart_callback_add(ad->parseButton1, "clicked", btn_parseFile_clicked_cb, ad->nf);
	evas_object_show(ad->parseButton1);
	elm_box_pack_end(box, ad->parseButton1);


	ad->parseButton2 = elm_button_add(box);
	elm_object_text_set(ad->parseButton2, "Parse from Array");
	evas_object_size_hint_min_set(ad->parseButton2, ELM_SCALE_SIZE(140), ELM_SCALE_SIZE(40));
	evas_object_smart_callback_add(ad->parseButton2, "clicked", btn_parseArray_clicked_cb, ad->nf);
	evas_object_show(ad->parseButton2);
	elm_box_pack_end(box, ad->parseButton2);
//----END

	elm_object_content_set(scroller, box);
	elm_naviframe_item_push(ad->nf, "JSON PARSER", NULL, NULL, scroller, NULL);

	elm_object_part_content_set(ad->layout, "elm.swallow.content", ad->nf);
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;
	create_base_gui(ad);
	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

    return ret;
}
