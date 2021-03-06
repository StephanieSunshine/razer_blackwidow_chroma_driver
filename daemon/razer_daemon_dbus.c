#include "razer_daemon.h"

#ifdef USE_DBUS

int daemon_dbus_error_check(char*message,DBusError *error)
{
	if(dbus_error_is_set(error))
	{
		#ifdef USE_DEBUGGING
		printf("dbus (%s) error:%s\n",message,error->message);
		#endif
		dbus_error_free(error);
		return(0);
	}
	return(1);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

dbus_bool_t daemon_dbus_check_user_handler(DBusConnection *connection, unsigned long uid, void *data)
{
	#ifdef USE_DEBUGGING
		printf("dbus: authenticated user:%lu\n",uid);
	#endif
	/*no user id check performed*/
	return(TRUE);
}

#pragma GCC diagnostic pop

int daemon_dbus_open(struct razer_daemon *daemon)
{
	DBusError error;
	dbus_error_init(&error);
	daemon->dbus = dbus_bus_get(DBUS_BUS_SYSTEM,&error);
	if(!daemon_dbus_error_check("open",&error))
		return(0);
	if(!daemon->dbus)
		return(0);
	dbus_connection_set_unix_user_function(daemon->dbus,daemon_dbus_check_user_handler,NULL,NULL);
	dbus_connection_set_allow_anonymous(daemon->dbus,TRUE);
	dbus_connection_set_route_peer_messages(daemon->dbus,TRUE);
	if(!daemon_dbus_error_check("open_user_check",&error))
		return(0);
	return(1);
}

void daemon_dbus_close(struct razer_daemon *daemon)
{
	if(daemon->dbus)
		dbus_connection_unref(daemon->dbus);
}

int daemon_dbus_add_method(struct razer_daemon *daemon,char *interface_name, char *method_name)
{
	//cat interface_name + method_name
	DBusError error;
	dbus_error_init(&error);
	char *match_front = "type='method_call',interface='";
	char *match = str_CreateEmpty();
	match = str_CatFree(match,match_front);
	match = str_CatFree(match,interface_name);
	match = str_CatFree(match,".");
	match = str_CatFree(match,method_name);
	match = str_CatFree(match,"'");
	#ifdef USE_VERBOSE_DEBUGGING
	printf("adding dbus method_call: %s\n",match);
	#endif
	dbus_bus_add_match(daemon->dbus,match,&error);
	free(match);
	if(!daemon_dbus_error_check("add_match",&error))
		return(0);
	return(1);
}

int daemon_dbus_announce(struct razer_daemon *daemon)
{
	DBusError error;
	dbus_error_init(&error);
	dbus_bus_request_name(daemon->dbus,"org.voyagerproject.razer.daemon",DBUS_NAME_FLAG_REPLACE_EXISTING,&error);
	if(!daemon_dbus_error_check("request_name",&error))
		return(0);
	//if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.fx","set"))
	//	return(0);
	//if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.fx","get"))
	//	return(0);
	//if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.fx.instances","list"))
	//	return(0);
	//if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.fx.libs","list"))
	//	return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.fps","set"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.fps","get"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.fx","list"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.fx.lib","load"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_nodes","list"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_nodes","render_list"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node","create"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node","set")) //TODO Remove 
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.opacity","set"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.opacity","get")) 
		return(0);
	//if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.output","connect"))
	//	return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.input","connect"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.second_input","connect"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.next","set"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.next","get"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.parent","get"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.next.move_frame_buffer_linkage","set"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.next.move_frame_buffer_linkage","get"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.sub","add"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.subs","list"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.parameters","list"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.parameter","set"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.parameter","get"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.render_node.parameter.value","get"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.frame_buffer","connect"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.frame_buffer","disconnect"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon","pause"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon","is_paused"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon","continue"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon.frame_buffer","get"))
		return(0);
	if(!daemon_dbus_add_method(daemon,"org.voyagerproject.razer.daemon","quit"))
		return(0);
	return(1);
}

int daemon_dbus_get_string_array_len(char **path)
{
	int i = 0;
	while(path[i]!=NULL)
		i++;
	return(i);
}


int daemon_dbus_handle_messages(struct razer_daemon *daemon)
{
	DBusMessage *msg;
	DBusMessage *reply;
	DBusMessageIter parameters;
	dbus_connection_read_write(daemon->dbus,0);
	msg = dbus_connection_pop_message(daemon->dbus);
	if(!msg)
		return(0);
	//printf("dbus: received message:type:%d ,path:%s ,interface:%s ,member:%s\n",dbus_message_get_type(msg),dbus_message_get_path(msg),dbus_message_get_interface(msg),dbus_message_get_member(msg));
    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_nodes", "list")) 
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method list render_nodes called\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		char *rn_list_json = str_CreateEmpty();
		rn_list_json = str_CatFree(rn_list_json,"{\n");
		rn_list_json = str_CatFree(rn_list_json," \"render_nodes_num\" : ");
		char *rn_num_string = str_FromLong(list_GetLen(daemon->fx_render_nodes));
		rn_list_json = str_CatFree(rn_list_json,rn_num_string);
		rn_list_json = str_CatFree(rn_list_json," ,\n");
		free(rn_num_string);
		rn_list_json = str_CatFree(rn_list_json," \"render_nodes_list\": [\n");
		for(int i=0;i<list_GetLen(daemon->fx_render_nodes);i++)
		{
			struct razer_fx_render_node *render_node = list_Get(daemon->fx_render_nodes,i);
			char *rn_json = daemon_render_node_to_json(render_node);
			rn_list_json = str_CatFree(rn_list_json,rn_json);
			free(rn_json);
		}
		rn_list_json = str_CatFree(rn_list_json,"]}\n");
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&rn_list_json)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
		free(rn_list_json);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_nodes", "render_list")) 
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method list rendering render_nodes called\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		char *rn_list_json = str_CreateEmpty();
		rn_list_json = str_CatFree(rn_list_json,"{\n");
		rn_list_json = str_CatFree(rn_list_json," \"render_nodes_num\" : ");
		char *rn_num_string = str_FromLong(list_GetLen(daemon->render_nodes));
		rn_list_json = str_CatFree(rn_list_json,rn_num_string);
		rn_list_json = str_CatFree(rn_list_json," ,\n");
		free(rn_num_string);
		rn_list_json = str_CatFree(rn_list_json," \"render_nodes_list\": [\n");
		for(int i=0;i<list_GetLen(daemon->render_nodes);i++)
		{
			struct razer_fx_render_node *render_node = list_Get(daemon->render_nodes,i);
			char *rn_json = daemon_render_node_to_json(render_node);
			rn_list_json = str_CatFree(rn_list_json,rn_json);
			free(rn_json);
		}
		rn_list_json = str_CatFree(rn_list_json,"]}\n");
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&rn_list_json)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
		free(rn_list_json);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.parameters", "list")) 
	{
		char **path = NULL;
		int rn_uid = -1;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method list render_nodes.parameters called for : %s\n",path[0]);
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		char *rn_list_json = str_CreateEmpty();
		dbus_free_string_array(path);
		struct razer_fx_render_node *render_node = daemon_get_render_node(daemon,rn_uid);
		if(render_node)
		{
			rn_list_json = str_CatFree(rn_list_json,"{\n");
			rn_list_json = str_CatFree(rn_list_json," \"parameters_num\" : ");
			char *rn_num_string = str_FromLong(render_node->effect->parameters->num);
			rn_list_json = str_CatFree(rn_list_json,rn_num_string);
			rn_list_json = str_CatFree(rn_list_json," ,\n");
			free(rn_num_string);
			rn_list_json = str_CatFree(rn_list_json," \"parameters_list\": [\n");
			for(int i=0;i<render_node->effect->parameters->num;i++)
			{
				char *rn_json = daemon_parameter_to_json(render_node->effect->parameters->items[i]);
				rn_list_json = str_CatFree(rn_list_json,rn_json);
				free(rn_json);
			}
			rn_list_json = str_CatFree(rn_list_json,"]}\n");
		}
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&rn_list_json)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
		free(rn_list_json);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.parameter", "get"))
	{
		char **path = NULL;
		int rn_uid = -1;
		int p_index = -1;
		int a_index = -1;
		dbus_message_get_path_decomposed(msg,&path);
		int path_len = daemon_dbus_get_string_array_len(path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method get render_nodes.parameter called for : %d\n",path_len);
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		if(path_len >= 2)
		{
			rn_uid = atoi(path[0]);
			p_index = atoi(path[1]);
			#ifdef USE_DEBUGGING
				printf("dbus: get parameter path : %d , %d\n",rn_uid,p_index);
				printf("dbus: get parameter max path render_node : %d\n",list_GetLen(daemon->fx_render_nodes));
			#endif
		}
		if(path_len == 3)
		{
			a_index = atoi(path[2]);
		}
			
		char *rn_list_json = str_CreateEmpty();
		dbus_free_string_array(path);
		if(list_GetLen(daemon->fx_render_nodes))
		{
			//struct razer_fx_render_node *render_node = daemon->fx_render_nodes[rn_uid];
			struct razer_fx_render_node *render_node = daemon_get_render_node(daemon,rn_uid);
			if(render_node && p_index>=0 && p_index < list_GetLen(render_node->effect->parameters))
			{
				struct razer_parameter *parameter = list_Get(render_node->effect->parameters,p_index);
				if(a_index != -1 && (parameter->type == RAZER_PARAMETER_TYPE_INT_ARRAY || parameter->type == RAZER_PARAMETER_TYPE_UINT_ARRAY || parameter->type == RAZER_PARAMETER_TYPE_FLOAT_ARRAY || parameter->type == RAZER_PARAMETER_TYPE_POS_ARRAY || parameter->type == RAZER_PARAMETER_TYPE_RGB_ARRAY))
				{
					char *parameter_json = daemon_parameter_array_to_json(parameter,a_index);
					rn_list_json = str_CatFree(rn_list_json,parameter_json);
					free(parameter_json);
				}
				else
				{
					char *parameter_json = daemon_parameter_to_json(parameter);
					rn_list_json = str_CatFree(rn_list_json,parameter_json);
					free(parameter_json);
				}
			}
		}
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&rn_list_json)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
		free(rn_list_json);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.parameter", "set"))
	{
		char **path = NULL;
		int rn_uid = -1;
		int p_index = -1;
		int a_index = -1;
		dbus_message_get_path_decomposed(msg,&path);
		int path_len = daemon_dbus_get_string_array_len(path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method set render_nodes.parameter called for : %d\n",path_len);
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		if(path_len >= 2)
		{
			rn_uid = atoi(path[0]);
			p_index = atoi(path[1]);
			#ifdef USE_DEBUGGING
				printf("dbus: set parameter path : %d , %d\n",rn_uid,p_index);
				printf("dbus: set parameter max path render_node : %d\n",list_GetLen(daemon->fx_render_nodes));
			#endif
		}
		if(path_len == 3)
		{
			a_index = atoi(path[2]);
		}
		dbus_free_string_array(path);
		if(dbus_message_iter_init(msg, &parameters) && list_GetLen(daemon->fx_render_nodes))
		{
			//struct razer_fx_render_node *render_node = daemon->fx_render_nodes[rn_uid];
			struct razer_fx_render_node *render_node = daemon_get_render_node(daemon,rn_uid);
			
			if(render_node && p_index>=0 && p_index < list_GetLen(render_node->effect->parameters))
			{
				struct razer_parameter *parameter = list_Get(render_node->effect->parameters,p_index);
				switch(parameter->type)
				{
					case RAZER_PARAMETER_TYPE_STRING:
						if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_STRING)
						{
							char *fxname;
							dbus_message_iter_get_basic(&parameters,&fxname);
							#ifdef USE_DEBUGGING
								printf("dbus: string parameter:%s\n",fxname);
							#endif
						}
						break;
					case RAZER_PARAMETER_TYPE_FLOAT:
						if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_DOUBLE)
						{
							double value = 0;
							dbus_message_iter_get_basic(&parameters,&value);
							#ifdef USE_DEBUGGING
								printf("dbus: float parameter:%d\n",value);
							#endif
							daemon_set_parameter_float(parameter,value);
						}
						break;
					case RAZER_PARAMETER_TYPE_INT:
						if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
						{
							long value = 0;
							dbus_message_iter_get_basic(&parameters,&value);
							#ifdef USE_DEBUGGING
								printf("dbus: int parameter:%d\n",value);
							#endif
							daemon_set_parameter_int(parameter,value);
						}
						break;
					case RAZER_PARAMETER_TYPE_UINT:
						if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_UINT32)
						{
							unsigned long value = 0;
							dbus_message_iter_get_basic(&parameters,&value);
							#ifdef USE_DEBUGGING
								printf("dbus: unsigned int parameter:%d\n",value);
							#endif
							daemon_set_parameter_uint(parameter,value);
						}
						break;
					case RAZER_PARAMETER_TYPE_RGB:
						{
							struct razer_rgb *color = daemon_get_parameter_rgb(parameter);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: R parameter:%d\n",value);
								#endif
								color->r = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: G parameter:%d\n",value);
								#endif
								color->g = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: B parameter:%d\n",value);
								#endif
								color->b = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_POS:
						{
							struct razer_pos *pos = daemon_get_parameter_pos(parameter);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: X parameter:%d\n",value);
								#endif
								pos->x = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: Y parameter:%d\n",value);
								#endif
								pos->y = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_RENDER_NODE:
						{
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: Node id parameter:%d\n",value);
								#endif
								struct razer_fx_render_node *dst_rn = daemon_get_render_node(daemon,(int)value);
								if(dst_rn)
									daemon_set_parameter_render_node(parameter,dst_rn);
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_FLOAT_RANGE:
						{
							struct razer_float_range *range = daemon_get_parameter_float_range(parameter);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_DOUBLE)
							{
								double value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: min parameter:%f\n",value);
								#endif
								range->min = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_DOUBLE)
							{
								double value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: max parameter:%f\n",value);
								#endif
								range->max = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_INT_RANGE:
						{
							struct razer_int_range *range = daemon_get_parameter_int_range(parameter);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: min parameter:%l\n",value);
								#endif
								range->min = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: max parameter:%l\n",value);
								#endif
								range->max = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_UINT_RANGE:
						{
							struct razer_uint_range *range = daemon_get_parameter_uint_range(parameter);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_UINT32)
							{
								unsigned long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: min parameter:%ul\n",value);
								#endif
								range->min = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_UINT32)
							{
								unsigned long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: max parameter:%l\n",value);
								#endif
								range->max = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_RGB_RANGE:
						{
							struct razer_rgb_range *range = daemon_get_parameter_rgb_range(parameter);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: min R parameter:%d\n",value);
								#endif
								range->min->r = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: min G parameter:%d\n",value);
								#endif
								range->min->g = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: min B parameter:%d\n",value);
								#endif
								range->min->b = value;
							}
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: max R parameter:%d\n",value);
								#endif
								range->max->r = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: max G parameter:%d\n",value);
								#endif
								range->max->g = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: max B parameter:%d\n",value);
								#endif
								range->max->b = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_POS_RANGE:
						{
							struct razer_pos_range *range = daemon_get_parameter_pos_range(parameter);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: min X parameter:%d\n",value);
								#endif
								range->min->x = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: min Y parameter:%d\n",value);
								#endif
								range->min->y = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: max X parameter:%d\n",value);
								#endif
								range->max->x = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: max Y parameter:%d\n",value);
								#endif
								range->max->y = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_FLOAT_ARRAY:
						{
							struct razer_float_array *array = daemon_get_parameter_float_array(parameter);
							if(a_index < 0 || a_index >= array->size)
								break;
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_DOUBLE)
							{
								double value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: float parameter:%f\n",value);
								#endif
								array->values[a_index] = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_INT_ARRAY:						
						{
							struct razer_int_array *array = daemon_get_parameter_int_array(parameter);
							if(a_index < 0 || a_index >= array->size)
								break;
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: integer parameter:%d\n",value);
								#endif
								array->values[a_index] = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_UINT_ARRAY:
						{
							struct razer_uint_array *array = daemon_get_parameter_uint_array(parameter);
							if(a_index < 0 || a_index >= array->size)
								break;
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_UINT32)
							{
								unsigned long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: integer parameter:%u\n",value);
								#endif
								array->values[a_index] = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_POS_ARRAY:
						{
							struct razer_pos_array *array = daemon_get_parameter_pos_array(parameter);
							if(a_index < 0 || a_index >= array->size)
								break;
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: integer parameter:%d\n",value);
								#endif
								array->values[a_index]->x = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: integer parameter:%d\n",value);
								#endif
								array->values[a_index]->y = value;
							}
						}
						break;
					case RAZER_PARAMETER_TYPE_RGB_ARRAY:
						{
							struct razer_rgb_array *array = daemon_get_parameter_rgb_array(parameter);
							if(a_index < 0 || a_index >= array->size)
								break;
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: integer parameter:%d\n",value);
								#endif
								array->values[a_index]->r = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: integer parameter:%d\n",value);
								#endif
								array->values[a_index]->g = value;
							}
							dbus_message_iter_next(&parameters);
							if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
							{
								long value = 0;
								dbus_message_iter_get_basic(&parameters,&value);
								#ifdef USE_DEBUGGING
									printf("dbus: integer parameter:%d\n",value);
								#endif
								array->values[a_index]->b = value;
							}
						}
						break;
				}
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node", "create"))
	{
		int fx_uid=0;
		char *name = NULL;
		char *description = NULL;
		reply = dbus_message_new_method_return(msg);

		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
			{
				dbus_message_iter_get_basic(&parameters,&fx_uid);
			}
			dbus_message_iter_next(&parameters);
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_STRING)
			{
				dbus_message_iter_get_basic(&parameters,&name);
			}
			dbus_message_iter_next(&parameters);
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_STRING)
			{
				dbus_message_iter_get_basic(&parameters,&description);
			}

			if(fx_uid && description && name)
			{
				dbus_message_iter_init_append(reply,&parameters);
				struct razer_fx_render_node *rn = daemon_create_render_node(daemon,daemon_get_effect(daemon,fx_uid),-1,-1,-1,name,description);
				daemon_register_render_node(daemon,rn);
				#ifdef USE_DEBUGGING
					printf("\ndbus: created render_node:%d\n",rn->id);
				#endif
				/*char *rn_uid_json = str_CreateEmpty();
				rn_uid_json = str_CatFree(rn_uid_json,"{\n");
				rn_uid_json = str_CatFree(rn_uid_json," \"uid\" : ");
				char *rn_uid_string = str_FromLong(rn->id);
				rn_uid_json = str_CatFree(rn_uid_json,rn_uid_string);
				rn_uid_json = str_CatFree(rn_uid_json,"\n");
				free(rn_uid_string);
				rn_uid_json = str_CatFree(rn_uid_json,"}\n");
				if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&rn_uid_json)) 
					daemon_kill(daemon,"dbus: Out Of Memory!\n");
				free(rn_uid_json);
				*/
				if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_INT32,&rn->id)) 
					daemon_kill(daemon,"dbus: Out Of Memory!\n");
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node", "set"))
	{
		int rn_uid=0;
		reply = dbus_message_new_method_return(msg);

		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
			{
				dbus_message_iter_get_basic(&parameters,&rn_uid);
			}
			if(rn_uid)
			{
				dbus_message_iter_init_append(reply,&parameters);
				struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
				#ifdef USE_DEBUGGING
					printf("\ndbus: setting render_node to: %d (@%x)\n",rn_uid,rn);
					//fflush(stdout);
				#endif
				if(rn)
					daemon->render_node = rn;
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.opacity", "set"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method set render_node.opacity called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		double opacity=-1.0f;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);

		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_DOUBLE)
			{
				dbus_message_iter_get_basic(&parameters,&opacity);
			}
			if(rn_uid && opacity != -1.0f)
			{
				dbus_message_iter_init_append(reply,&parameters);
				struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
				rn->opacity = opacity;
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.opacity", "get"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method get render_node.opacity called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);

		if(rn_uid)
		{
			dbus_message_iter_init_append(reply,&parameters);
			struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
			double opacity = rn->opacity;
			if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_DOUBLE,&opacity)) 
				daemon_kill(daemon,"dbus: Out Of Memory!\n");
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.input", "connect"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method connect render_node.input called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		int dst_rn_uid=0;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);

		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
			{
				dbus_message_iter_get_basic(&parameters,&dst_rn_uid);
			}
			if(rn_uid && dst_rn_uid)
			{
				dbus_message_iter_init_append(reply,&parameters);
				struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
				struct razer_fx_render_node *dst_rn = daemon_get_render_node(daemon,dst_rn_uid);
				#ifdef USE_DEBUGGING
					printf("dbus: connecting input frame buffer to render_node : %d (@%x) to %d (@%x)\n",rn_uid,rn,dst_rn_uid,dst_rn);
				#endif
				daemon_connect_input(daemon,rn,dst_rn);
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.second_input", "connect"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method connect render_node.second_input called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		int dst_rn_uid=0;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);

		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
			{
				dbus_message_iter_get_basic(&parameters,&dst_rn_uid);
			}
			if(rn_uid && dst_rn_uid)
			{
				dbus_message_iter_init_append(reply,&parameters);
				struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
				struct razer_fx_render_node *dst_rn = daemon_get_render_node(daemon,dst_rn_uid);
				#ifdef USE_DEBUGGING
					printf("dbus: connecting second input frame buffer to render_node : %d (@%x) to %d (@%x)\n",rn_uid,rn,dst_rn_uid,dst_rn);
				#endif
				daemon_connect_second_input(daemon,rn,dst_rn);
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.next", "set"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method set render_node.next called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		int dst_rn_uid=0;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);

		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
			{
				dbus_message_iter_get_basic(&parameters,&dst_rn_uid);
			}
			if(rn_uid && dst_rn_uid)
			{
				dbus_message_iter_init_append(reply,&parameters);
				struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
				struct razer_fx_render_node *dst_rn = daemon_get_render_node(daemon,dst_rn_uid);
				rn->next = dst_rn;
				#ifdef USE_DEBUGGING
				if(dst_rn->prev)
					printf("Warning: Render_node.prev already set.\n");
				#endif
				dst_rn->prev = rn;
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.next", "get"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method get render_node.next called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);

		if(rn_uid)
		{
			dbus_message_iter_init_append(reply,&parameters);
			struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
			if(rn->next)
			{
				int next_uid = rn->next->id;
				if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_INT32,&next_uid)) 
					daemon_kill(daemon,"dbus: Out Of Memory!\n");
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.next.move_frame_buffer_linkage", "set"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method set render_node.next.move_frame_buffer_linkage called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		int move_linkage=0;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);

		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
			{
				dbus_message_iter_get_basic(&parameters,&move_linkage);
			}
			if(rn_uid && (move_linkage == 1 || move_linkage == 0))
			{
				dbus_message_iter_init_append(reply,&parameters);
				struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
				rn->move_frame_buffer_linkage_to_next = move_linkage;
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.next.move_frame_buffer_linkage", "get"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method get render_node.next.move_frame_buffer_linkage called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);

		if(rn_uid)
		{
			dbus_message_iter_init_append(reply,&parameters);
			struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
			if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_INT32,&rn->move_frame_buffer_linkage_to_next)) 
				daemon_kill(daemon,"dbus: Out Of Memory!\n");
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.parent", "get"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method get render_node.parent called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);
		int parent_uid = 0;
		if(rn_uid)
		{
			dbus_message_iter_init_append(reply,&parameters);
			struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
			if(rn->parent)
				parent_uid = rn->parent->id;
		}
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_INT32,&parent_uid)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");

 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.limit_render_time_ms", "set"))
	{
		char **path = NULL;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method set render_node.limit_render_time_ms called for : %s\n",path[0]);
		#endif
		int rn_uid=0;
		int limit_render_time_ms=0;
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		dbus_free_string_array(path);
		reply = dbus_message_new_method_return(msg);

		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
			{
				dbus_message_iter_get_basic(&parameters,&limit_render_time_ms);
			}
			if(rn_uid && limit_render_time_ms)
			{
				dbus_message_iter_init_append(reply,&parameters);
				struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
				rn->limit_render_time_ms = limit_render_time_ms;
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.subs", "list")) 
	{
		char **path = NULL;
		int rn_uid = -1;
		dbus_message_get_path_decomposed(msg,&path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method list render_nodes.subs called for : %s\n",path[0]);
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		if(path[0] != NULL)
			rn_uid = atoi(path[0]);
		char *rn_list_json = str_CreateEmpty();
		dbus_free_string_array(path);
		struct razer_fx_render_node *render_node = daemon_get_render_node(daemon,rn_uid);
		if(render_node)
		{
			rn_list_json = str_CatFree(rn_list_json,"{\n");
			rn_list_json = str_CatFree(rn_list_json," \"subs_num\" : ");
			char *rn_num_string = str_FromLong(list_GetLen(render_node->subs));
			rn_list_json = str_CatFree(rn_list_json,rn_num_string);
			free(rn_num_string);
			if(list_GetLen(render_node->subs))
			{
				rn_list_json = str_CatFree(rn_list_json," ,\n");
				rn_list_json = str_CatFree(rn_list_json," \"subs_list\": [\n");
				for(int i=0;i<list_GetLen(render_node->subs);i++)
				{
					char *rn_json = daemon_render_node_to_json(list_Get(render_node->subs,i));
					rn_list_json = str_CatFree(rn_list_json,rn_json);
					free(rn_json);
				}
				rn_list_json = str_CatFree(rn_list_json,"]}\n");
			}
			else
				rn_list_json = str_CatFree(rn_list_json,"}\n");
		}
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&rn_list_json)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
		free(rn_list_json);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.render_node.sub", "add"))
	{
		char **path = NULL;
		int rn_uid = -1;
		int s_uid = -1;
		dbus_message_get_path_decomposed(msg,&path);
		int path_len = daemon_dbus_get_string_array_len(path);
		#ifdef USE_DEBUGGING
			printf("\ndbus: method add render_nodes.sub called for : %d\n",path_len);
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		if(path_len == 1)
		{
			rn_uid = atoi(path[0]);
			#ifdef USE_DEBUGGING
				printf("dbus: add sub path : %d\n",rn_uid);
			#endif
		}
		dbus_free_string_array(path);
		if(dbus_message_iter_init(msg, &parameters) && list_GetLen(daemon->fx_render_nodes))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
			{
				dbus_message_iter_get_basic(&parameters,&s_uid);
			}
			if(s_uid)
			{
				struct razer_fx_render_node *render_node = daemon_get_render_node(daemon,rn_uid);
				struct razer_fx_render_node *sub_render_node = daemon_get_render_node(daemon,s_uid);
				if(render_node && sub_render_node)
				{
					daemon_render_node_add_sub(render_node,sub_render_node);
				}
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.frame_buffer", "connect"))
	{
		int rn_uid=0;
		reply = dbus_message_new_method_return(msg);

		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
			{
				dbus_message_iter_get_basic(&parameters,&rn_uid);
			}
			if(rn_uid)
			{
				dbus_message_iter_init_append(reply,&parameters);
				struct razer_fx_render_node *rn = daemon_get_render_node(daemon,rn_uid);
				#ifdef USE_DEBUGGING
					printf("\ndbus: connecting output frame buffer to render_node to: %d (@%x)\n",rn_uid,rn);
				#endif
				daemon_connect_frame_buffer(daemon,rn);
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.frame_buffer", "disconnect"))
	{
		reply = dbus_message_new_method_return(msg);
		daemon_disconnect_frame_buffer(daemon);
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.frame_buffer", "get")) 
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method get frame_buffer called\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		/*char *rn_uid_json = str_CreateEmpty();
		rn_uid_json = str_CatFree(rn_uid_json,"{\n");
		rn_uid_json = str_CatFree(rn_uid_json," \"uid\" : ");
		char *rn_uid_string = str_FromLong(daemon->frame_buffer_linked_uid);
		rn_uid_json = str_CatFree(rn_uid_json,rn_uid_string);
		rn_uid_json = str_CatFree(rn_uid_json,"\n");
		free(rn_uid_string);
		rn_uid_json = str_CatFree(rn_uid_json,"}\n");
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&rn_uid_json)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		free(rn_uid_json);*/
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_INT32,&daemon->frame_buffer_linked_uid)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon", "pause")) 
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method pause called\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		daemon->is_paused = 1;
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon", "continue")) 
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method continue called\n");
		#endif
		daemon->is_paused = 0;
		reply = dbus_message_new_method_return(msg);
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.fps", "set")) 
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method set fps called\n");
		#endif
		if(!dbus_message_iter_init(msg, &parameters))
		{
			#ifdef USE_DEBUGGING
				printf("dbus: set fps received without parameter\n");
			#endif
		}
		if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_INT32)
		{
			dbus_message_iter_get_basic(&parameters,&daemon->fps);
			#ifdef USE_DEBUGGING
				printf("parameter:%d\n",daemon->fps);
			#endif
		}
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_INT32,&daemon->fps)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}
    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.fps", "get")) 
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method get fps called\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_INT32,&daemon->fps)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon", "is_paused")) 
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method is_paused called\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_INT32,&daemon->is_paused)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.fx", "list")) 
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method list fx called\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		char *fx_list_json = str_CreateEmpty();
		fx_list_json = str_CatFree(fx_list_json,"{\n");
		fx_list_json = str_CatFree(fx_list_json," \"effects_num\" : ");
		char *effects_num_string = str_FromLong(list_GetLen(daemon->effects));
		fx_list_json = str_CatFree(fx_list_json,effects_num_string);
		fx_list_json = str_CatFree(fx_list_json," ,\n");
		free(effects_num_string);
		fx_list_json = str_CatFree(fx_list_json," \"effects_list\": [\n");
		for(int i=0;i<list_GetLen(daemon->effects);i++)
		{
			struct razer_effect *effect = list_Get(daemon->effects,i);
			char *effect_json = daemon_effect_to_json(effect);
			fx_list_json = str_CatFree(fx_list_json,effect_json);
			free(effect_json);
		}
		fx_list_json = str_CatFree(fx_list_json,"]}\n");
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&fx_list_json)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
		free(fx_list_json);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.fx.lib", "load"))
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method load fx.lib called for\n");
		#endif
		char *lib_filename = NULL;
		reply = dbus_message_new_method_return(msg);
		if(dbus_message_iter_init(msg, &parameters))
		{
			if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_STRING)
			{
				dbus_message_iter_get_basic(&parameters,&lib_filename);
			}

			if(lib_filename)
			{
				void *lib = daemon_load_fx_lib(daemon,lib_filename);
				if(lib)
				{
					daemon_register_lib(daemon,lib);
				}
			}
		}
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon", "quit"))
	{
		#ifdef USE_DEBUGGING
			printf("\ndbus: method quit daemon called for\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		daemon->running = 0;
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}

	/*
    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.fx.libs.", "list")) 
	{
		#ifdef USE_DEBUGGING
			printf("dbus: method list fx libs called\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		char *libs_list_json = str_CreateEmpty();
		libs_list_json = str_CatFree(libs_list_json,"{\n");
		libs_list_json = str_CatFree(libs_list_json," \"libs_num\" : ");
		char *libs_num_string = str_FromLong(daemon->libs_num);
		libs_list_json = str_CatFree(libs_list_json,libs_num_string);
		libs_list_json = str_CatFree(libs_list_json," ,\n");
		free(libs_num_string);
		libs_list_json = str_CatFree(libs_list_json," \"libs_list\": [\n");
		for(int i=0;i<daemon->libs_num;i++)
		{
			struct razer_effect *effect = daemon->effects[i];
			char *effect_json = daemon_effect_to_json(effect);
			libs_list_json = str_CatFree(libs_list_json,effect_json);
			free(effect_json);
		}
		libs_list_json = str_CatFree(libs_list_json,"]}\n");
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&libs_list_json)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
		free(libs_list_json);
	}
	*/


	/*
    if(dbus_message_is_method_call(msg, "org.voyagerproject.razer.daemon.fx", "set")) 
	{
		#ifdef USE_DEBUGGING
			printf("dbus: method set fx called\n");
		#endif
		if(!dbus_message_iter_init(msg, &parameters))
		{
			#ifdef USE_DEBUGGING
				printf("dbus: signal set fx received without parameter\n");
			#endif
		}
		if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_STRING)
		{
			char *fxname;
			dbus_message_iter_get_basic(&parameters,&fxname);
			printf("parameter:%s\n",fxname);
		}

		struct razer_rgb col={.r=5,.g=0,.b=0};
		for(int i=0;i<250;i++)
		{
			col.r = 5+(i*1);
			razer_set_all(daemon->chroma->keys,&col);
			razer_update_keys(daemon->chroma,daemon->chroma->keys);
			//usleep((1000*1000)/255);
		}
		reply = dbus_message_new_method_return(msg);
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
	}
	*/

    if(dbus_message_is_method_call(msg, "org.freedesktop.DBus.Introspectable", "Introspect")) 
	{
		#ifdef USE_DEBUGGING
			printf("dbus: method Introspect called\n");
		#endif
		reply = dbus_message_new_method_return(msg);
		dbus_message_iter_init_append(reply,&parameters);
		char *xml_data_start = 
		"<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\"\n\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">\n\
		<node>\n\
			<interface name=\"org.freedesktop.DBus.Introspectable\">\n\
				<method name=\"Introspect\">\n\
					<arg direction=\"out\" name=\"data\" type=\"s\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon\">\n\
				<method name=\"quit\">\n\
				</method>\n\
				<method name=\"pause\">\n\
				</method>\n\
				<method name=\"is_paused\">\n\
					<arg direction=\"out\" name=\"is_paused\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"continue\">\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.fps\">\n\
				<method name=\"set\">\n\
					<arg direction=\"in\" name=\"fps_rate\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"get\">\n\
					<arg direction=\"out\" name=\"fps_rate\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.fx\">\n\
				<method name=\"list\">\n\
					<arg direction=\"out\" name=\"fx_list_json\" type=\"s\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"load\">\n\
					<arg direction=\"in\" name=\"fx_so_pathname\" type=\"s\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_nodes\">\n\
				<method name=\"list\">\n\
					<arg direction=\"out\" name=\"render_nodes_list_json\" type=\"s\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"render_list\">\n\
					<arg direction=\"out\" name=\"render_list_render_nodes_list_json\" type=\"s\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node\">\n\
				<method name=\"set\">\n\
					<arg direction=\"in\" name=\"render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"create\">\n\
					<arg direction=\"in\" name=\"effect_uid\" type=\"i\">\n\
					</arg>\n\
					<arg direction=\"in\" name=\"name\" type=\"s\">\n\
					</arg>\n\
					<arg direction=\"in\" name=\"description\" type=\"s\">\n\
					</arg>\n\
					<arg direction=\"out\" name=\"render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.parameters\">\n\
				<method name=\"list\">\n\
					<arg direction=\"out\" name=\"parameters_list_json\" type=\"s\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.parameter\">\n\
				<method name=\"set\">\n\
					<arg direction=\"in\" name=\"parameter_value\" type=\"v\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"get\">\n\
					<arg direction=\"out\" name=\"parameter_value\" type=\"v\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n"; //split done to remove warning
			char *xml_data_end = "<interface name=\"org.voyagerproject.razer.daemon.render_node.opacity\">\n\
				<method name=\"set\">\n\
					<arg direction=\"in\" name=\"opacity\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"get\">\n\
					<arg direction=\"out\" name=\"opacity\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.input\">\n\
				<method name=\"connect\">\n\
					<arg direction=\"in\" name=\"render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.second_input\">\n\
				<method name=\"connect\">\n\
					<arg direction=\"in\" name=\"render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.next\">\n\
				<method name=\"set\">\n\
					<arg direction=\"in\" name=\"render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"get\">\n\
					<arg direction=\"out\" name=\"render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.next.move_frame_buffer_linkage\">\n\
				<method name=\"set\">\n\
					<arg direction=\"in\" name=\"move_linkage_bool\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"get\">\n\
					<arg direction=\"out\" name=\"move_linkage_bool\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.parent\">\n\
				<method name=\"get\">\n\
					<arg direction=\"out\" name=\"render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.limit_render_time_ms\">\n\
				<method name=\"set\">\n\
					<arg direction=\"in\" name=\"limit_render_time_ms\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.subs\">\n\
				<method name=\"list\">\n\
					<arg direction=\"out\" name=\"render_node_subs_list_json\" type=\"s\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.render_node.sub\">\n\
				<method name=\"add\">\n\
					<arg direction=\"in\" name=\"compute_render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
			<interface name=\"org.voyagerproject.razer.daemon.frame_buffer\">\n\
				<method name=\"connect\">\n\
					<arg direction=\"in\" name=\"render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
				<method name=\"disconnect\">\n\
				</method>\n\
				<method name=\"get\">\n\
					<arg direction=\"out\" name=\"render_node_uid\" type=\"i\">\n\
					</arg>\n\
				</method>\n\
			</interface>\n\
		</node>\n";
/*			<interface name=\"org.freedesktop.DBus.Properties\">\n\
				<method name=\"Get\">\n\
					<arg direction=\"in\" name=\"interface\" type=\"s\">\n\
					<arg direction=\"in\" name=\"propname\" type=\"s\">\n\
					<arg direction=\"out\" name=\"value\" type=\"v\">\n\
				</arg></arg></arg></method>\n\
				<method name=\"Set\">\n\
					<arg direction=\"in\" name=\"interface\" type=\"s\">\n\
					<arg direction=\"in\" name=\"propname\" type=\"s\">\n\
					<arg direction=\"in\" name=\"value\" type=\"v\">\n\
				</arg></arg></arg></method>\n\
				<method name=\"GetAll\">\n\
					<arg direction=\"in\" name=\"interface\" type=\"s\">\n\
					<arg direction=\"out\" name=\"props\" type=\"a{sv}\">\n\
				</arg></arg></method>\n\
			</interface>\n\
*/
		char *xml_data = str_CreateEmpty();
		xml_data = str_CatFree(xml_data,xml_data_start);
		xml_data = str_CatFree(xml_data,xml_data_end);
		if(!dbus_message_iter_append_basic(&parameters,DBUS_TYPE_STRING,&xml_data)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
 		dbus_uint32_t serial = 0;
 		if(!dbus_connection_send(daemon->dbus,reply,&serial)) 
			daemon_kill(daemon,"dbus: Out Of Memory!\n");
		dbus_connection_flush(daemon->dbus);
		free(xml_data);
	}

	/*
	if(dbus_message_is_signal(msg,"org.voyagerproject.razer.daemon.fx.set.Type","set"))
	{
		#ifdef USE_DEBUGGING
			printf("dbus: signal set fx received\n");
		#endif
		if(!dbus_message_iter_init(msg, &parameters))
		{
			#ifdef USE_DEBUGGING
				printf("dbus: signal set fx received without parameter\n");
			#endif
		}
		if(dbus_message_iter_get_arg_type(&parameters) == DBUS_TYPE_STRING)
		{
			char *fxname;
			dbus_message_iter_get_basic(&parameters,&fxname);
			printf("parameter:%s\n",fxname);
		}
	}
	*/
	dbus_message_unref(msg);
	return(1);
}

//end of dbus ifdef
#endif 

