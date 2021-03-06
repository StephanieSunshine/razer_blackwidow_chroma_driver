#include "razer_daemon_controller.h"




const char *dc_helpmsg = "Usage: %s [OPTIONS]... [COMMAND] [PARAMETERS]...\n\
Send commands to razer_bcd daemon.\n\
\n\
Commands:\n\
  -q    Close daemon\n\
  -c    Continue rendering\n\
  -p    Pause rendering\n\
  -C    Create rendering node\n\
           1. Parameter: Effect uid - sets effect render node uses\n\
           2. Parameter: Name - sets the render nodes name\n\
           3. Parameter: Description - sets the render nodes description\n\
\n\
           For example: %s -C 1 \"My render node\" \"My description\"\n\
  -l    Load fx library\n\
           1. Parameter: Library filename\n\
  -f    Set fps rate\n\
           1. Parameter: fps rate\n\
  -g    Get fps rate\n\
           Returns: actual fps rate of daemon\n\
  -o    Set render node opacity\n\
           1. Parameter: render node uid - render node to set the opacity to\n\
           2. Parameter: opacity value - float value between 0.0 and 1.0\n\
  -O    Get render node opacity\n\
           1. Parameter: render node uid - render node to get the opacity of\n\
           Returns: opacity of render node as a float value between 0.0 and 1.0\n\
  -i    Is rendering paused ?\n\
           Returns: 0/1 running/paused\n\
  -x    Get fx list\n\
           Returns: fx list as json string\n\
  -X    Get render nodes list\n\
           Returns: render nodes list as json string\n\
  -R    Get rendering nodes list\n\
           Returns: rendering nodes list as json string\n\
  -U    Get sub nodes list\n\
           1. Parameter: render node uid - render node to get the sub nodes of\n\
           Returns: sub nodes list as json string\n\
  -F    Get render node parameters list\n\
           1. Parameter: render node uid - render node to get the parameters of\n\
           Returns: render node parameter list as json string\n\
  -a    Get the actual render node uid connected to the framebuffer\n\
           Returns: uid of node\n\
  -t    Get the parent of a render node\n\
           1. Parameter: render node uid - render node to get the parent of\n\
           Returns: uid of parent node\n\
  -L    Set render node rendering time limit\n\
           1. Parameter: render node uid - render node to set the time limit\n\
           2. Parameter: time limit value - time span in ms\n\
  -b    Connect frame buffer to render node\n\
           1. Parameter: render node uid - render node that gets connected to the frame buffer\n\
  -s    Add Sub-node to render node\n\
           1. Parameter: render node uid - render node the sub node should be added to\n\
           2. Parameter: sub node uid - sub node that gets added\n\
  -r    Connect input node to render nodes first input slot\n\
           1. Parameter: render node uid - render node the input node should be connected to\n\
           2. Parameter: input node uid - input node to connect\n\
  -n    Connect input node to render nodes second input slot\n\
           1. Parameter: render node uid - render node the input node should be connected to\n\
           2. Parameter: input node uid - input node to connect\n\
  -w    Get the next node of a render node\n\
           1. Parameter: render node uid - render node to get the next node of\n\
           Returns: uid of next node\n\
  -y    Set the next node of a render node\n\
           1. Parameter: render node uid - render node to set the next node of\n\
           2. Parameter: next node uid - next node to run after render node finished\n\
  -M    Get the move_linkage value of a render node\n\
           1. Parameter: render node uid - render node to get the move_linkage value of\n\
           Returns: uid of next node\n\
  -G    Set the move_linkage value of a render node\n\
           1. Parameter: render node uid - render node to get the next node of\n\
           2. Parameter: move_linkage - 0/1 activate/deactivate moving of framebuffer\n\
                         linkage of a render node\n\
  -P    Get the parameter of a render node\n\
           1. Parameter: render node uid - render node the parameter belongs to\n\
           2. Parameter: parameter index - index of parameter to get\n\
           3. Parameter: array index - if parameter is an array this index will be used (optional)\n\
           Returns: parameter as json\n\
  -S    Set the parameter of a render node\n\
           1. Parameter: render node uid - render node the parameter belongs to\n\
           2. Parameter: parameter index - index of parameter to set\n\
           3. Parameter: array index - if parameter is an array this index will be used (use -1 to skip)\n\
           4. Parameter: parameter value - value to set\n\
  -d    Disconnect frame buffer\n\
  -h    Display this help and exit\n\
\n\
Options:\n\
  -v    More verbose output\n\
\n\
	DBUS must be running on the system to communicate with daemon.\n\
\n\
      Report bugs to <pez2001@voyagerproject.de>.\n";

int verbose = 0;


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int main(int argc,char *argv[])
{
	#ifndef USE_DBUS
		printf("You need to have dbus & dbus dev packages installed\n");
		return(1);
	#else
	char c;
	struct razer_daemon_controller *controller=NULL;
	if(!(controller=dc_open()))
	{
		printf("razer_bcd_controller: error initializing daemon controller\n");
		return(1);
	}
	
	int opts_given = 0;
	while((c=getopt(argc,argv,"hvVcpqlfoigatOLxbdsrnwyCMGPSXRUF")) != -1)
	{
		opts_given = 1;
		switch(c)
		{
			case 'P':
				{
					int render_node_uid = atoi(argv[optind++]);
					int parameter_index = atoi(argv[optind++]);
					int array_index = -1;
					if(optind < argc)
						array_index = atoi(argv[optind++]);
					char *parameter_json = dc_render_node_parameter_get(controller,render_node_uid,parameter_index,array_index);
					if(verbose)
					{
						printf("sending get parameter value of render node: %d.%d.%d.\n",render_node_uid,parameter_index,array_index);
						printf("value: %s.\n",parameter_json);
					}
					else
						printf("%s",parameter_json);
					//free(parameter_json);
				}
			case 'S':
				{
					int render_node_uid = atoi(argv[optind++]);
					int parameter_index = atoi(argv[optind++]); // TODO switch to parameter uid someday
					//int parameter_uid = atoi(argv[optind++]);
					int	array_index = atoi(argv[optind++]);
					char *type = argv[optind++];
					char *value = argv[optind++];
					if(verbose)
						printf("sending set parameter value of render node: %d.%d.%d = [%s].\n",render_node_uid,parameter_index,array_index,value);
					dc_render_node_parameter_parsed_set(controller,render_node_uid,parameter_index,array_index,type,value);
				}
				break;
			case 'M':
				{
					int render_node_uid = atoi(argv[optind++]);
					int move_linkage = atoi(argv[optind]);
					if(verbose)
						printf("sending set move_linkage to %d for render node: %d command to daemon.\n",move_linkage,render_node_uid);
					dc_render_node_next_move_frame_buffer_linkage_set(controller,render_node_uid,move_linkage);
				}
				break;
			case 'G':
				{
					int render_node_uid = atoi(argv[optind++]);
					if(verbose)
					{
						printf("sending get move_linkage of render node: %d.\n",render_node_uid);
						printf("move linkage of render node: %d.\n",dc_render_node_next_move_frame_buffer_linkage_get(controller,render_node_uid));
					}
					else
						printf("%d",dc_render_node_next_move_frame_buffer_linkage_get(controller,render_node_uid));
				}
				break;
			case 'C':
				{
					int fx_uid = atoi(argv[optind++]);
					char *node_name = argv[optind++];
					char *node_description = argv[optind++];
					if(verbose)
					{
						printf("sending create render node command to daemon.\n");
						printf("new render node uid: %d.\n",dc_render_node_create(controller,fx_uid,node_name,node_description));
					}
					else
						printf("%d",dc_render_node_create(controller,fx_uid,node_name,node_description));

				}
				break;
			case 'y':
				{
					int render_node_uid = atoi(argv[optind++]);
					int next_node_uid = atoi(argv[optind]);
					if(verbose)
						printf("sending set next node : %d for render node: %d command to daemon.\n",next_node_uid,render_node_uid);
					dc_render_node_next_set(controller,render_node_uid,next_node_uid);
				}
				break;
			case 'w':
				{
					int render_node_uid = atoi(argv[optind++]);
					if(verbose)
					{
						printf("sending get next node of render node: %d.\n",render_node_uid);
						printf("next render node: %d.\n",dc_render_node_next_get(controller,render_node_uid));
					}
					else
						printf("%d",dc_render_node_next_get(controller,render_node_uid));
				}
				break;
			case 'r':
				{
					int render_node_uid = atoi(argv[optind++]);
					int input_node_uid = atoi(argv[optind]);
					if(verbose)
						printf("sending connect node : %d to render nodes: %d first input command to daemon.\n",input_node_uid,render_node_uid);
					dc_render_node_input_connect(controller,render_node_uid,input_node_uid);
				}
				break;
			case 'n':
				{
					int render_node_uid = atoi(argv[optind++]);
					int input_node_uid = atoi(argv[optind]);
					if(verbose)
						printf("sending connect node : %d to render nodes: %d second input command to daemon.\n",input_node_uid,render_node_uid);
					dc_render_node_second_input_connect(controller,render_node_uid,input_node_uid);
				}
				break;
			case 's':
				{
					int render_node_uid = atoi(argv[optind++]);
					int sub_node_uid = atoi(argv[optind]);
					if(verbose)
						printf("sending add sub node: %d to render node: %d command to daemon.\n",sub_node_uid,render_node_uid);
					dc_render_node_sub_add(controller,render_node_uid,sub_node_uid);
				}
				break;
			case 'b':
				{
					int render_node_uid = atoi(argv[optind]);
					if(verbose)
						printf("sending connect frame buffer to render node: %d command to daemon.\n",render_node_uid);
					dc_frame_buffer_connect(controller,render_node_uid);
				}
				break;
			case 'd':
				{
					if(verbose)
						printf("sending disconnect frame buffer command to daemon.\n");
					dc_frame_buffer_disconnect(controller);
				}
				break;
			case 'x':
				{
					char *list = dc_fx_list(controller);
					if(verbose)
					{	
						printf("sending get effects list command to daemon.\n");
						printf("daemon fx list:\n%s.\n",list);
					}
					else
						printf("%s",list);
					free(list);
				}
				break;
			case 'X':
				{
					char *list = dc_render_nodes_list(controller);
					if(verbose)
					{	
						printf("sending get render nodes list command to daemon.\n");
						printf("daemon render nodes list:\n%s.\n",list);
					}
					else
						printf("%s",list);
					free(list);
				}
				break;
			case 'R':
				{
					char *list = dc_rendering_nodes_list(controller);
					if(verbose)
					{	
						printf("sending get rendering nodes list command to daemon.\n");
						printf("daemon rendering nodes list:\n%s.\n",list);
					}
					else
						printf("%s",list);
					free(list);
				}
				break;
			case 'U':
				{
					int render_node_uid = atoi(argv[optind++]);
					char *list = dc_sub_nodes_list(controller,render_node_uid);
					if(verbose)
					{	
						printf("sending get sub nodes list command to daemon.\n");
						printf("daemon sub nodes list:\n%s.\n",list);
					}
					else
						printf("%s",list);
					free(list);
				}
				break;
			case 'F':
				{
					int render_node_uid = atoi(argv[optind++]);
					char *list = dc_render_node_parameters_list(controller,render_node_uid);
					if(verbose)
					{	
						printf("sending get render node parameters list command to daemon.\n");
						printf("daemon render node parameters list:\n%s.\n",list);
					}
					else
						printf("%s",list);
					free(list);
				}
				break;
			case 'L':
				{
					int render_node_uid = atoi(argv[optind++]);
					int time_limit_ms = atoi(argv[optind]);
					if(verbose)
						printf("sending render node limit render time command to daemon.\n");
					dc_render_node_limit_render_time_ms_set(controller,render_node_uid,time_limit_ms);
				}
				break;
			case 't':
				{
					int render_node_uid = atoi(argv[optind]);
					if(verbose)
					{
						printf("sending get parent render node command to daemon.\n");
						printf("render node parent:%d.\n",dc_render_node_parent_get(controller,render_node_uid));
					}
					else
						printf("%d",dc_render_node_parent_get(controller,render_node_uid));
				}
				break;
			case 'a':
				if(verbose)
				{
					printf("sending get framebuffer connected render node command to daemon.\n");
					printf("daemon is running node:%d.\n",dc_frame_buffer_get(controller));
				}
				else
					printf("%d",dc_frame_buffer_get(controller));
				break;
			case 'g':
				if(verbose)
				{
					printf("sending get fps command to daemon.\n");
					printf("daemon is running at %d fps.\n",dc_fps_get(controller));
				}
				else
					printf("%d",dc_fps_get(controller));
				break;
			case 'i':
				if(verbose)
				{
					printf("sending is paused command to daemon.\n");
					int paused = dc_is_paused(controller);
					if(paused)
						printf("daemon is paused.\n");
					else
						printf("daemon is running.\n");
				}
				else
					printf("%d",dc_is_paused(controller));
				break;
			case 'o':
				{
					int render_node_uid = atoi(argv[optind++]);
					double opacity = atof(argv[optind]);
					if(verbose)
						printf("sending set opacity for render node: %d command to daemon: %f.\n",render_node_uid,opacity);
					dc_render_node_opacity_set(controller,render_node_uid,opacity);
				}
				break;
			case 'O':
				{
					int render_node_uid = atoi(argv[optind]);
					if(verbose)
					{
						printf("sending get opacity for render node: %d command to daemon.\n",render_node_uid);
						printf("render node opacity is to: %f.\n",dc_render_node_opacity_get(controller,render_node_uid));
					}
					else
						printf("%f",dc_render_node_opacity_get(controller,render_node_uid));
				}
				break;
			case 'f':
				{
					int fps = atoi(argv[optind]);
					if(verbose)
						printf("sending set fps command to daemon: %d.\n",fps);
					dc_fps_set(controller,fps);
				}
				break;
			case 'l':
				if(verbose)
					printf("sending load fx library command to daemon: library to load: \"%s\".\n",argv[optind]);
				dc_load_fx_lib(controller,argv[optind]);
				break;
			case 'c':
				if(verbose)
					printf("sending continue command to daemon.\n");
				dc_continue(controller);
				break;
			case 'p':
				if(verbose)
					printf("sending pause command to daemon.\n");
				dc_pause(controller);
				break;
			case 'q':
				if(verbose)
					printf("sending quit command to daemon.\n");
				dc_quit(controller);
				break;
			case 'v':
				verbose = 1;
				break;
			case 'h':
				printf("Razer blackwidow chroma daemon controller\n");
				printf(dc_helpmsg,argv[0],argv[0]);
				return(0);
			case 'V':
				printf("razer_bcd daemon controller %d.%d (build %d)\n",MAJOR_VERSION,MINOR_VERSION);
				return(0);
			case '?':
				if(optopt == 'c')
					printf("Option -%c requires an argument.\n",optopt);
				else if(isprint(optopt))
					printf("Unknown option `-%c'.\n",optopt);
				else
					printf("Unknown option character `\\x%x'.\n",optopt);
				opts_given = 0;
				return(1);
			default:
				opts_given = 0;
				abort();
		}
	}
	dc_close(controller);
	if(!opts_given)
	{
		printf("Razer blackwidow chroma daemon controller\n");
		printf(dc_helpmsg,argv[0],argv[0]);
	}		
	#endif
	return(0);
}

#pragma GCC diagnostic pop

