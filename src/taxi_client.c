#include <gtk/gtk.h>
/* #include "config.h" */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdint.h>
#include <assert.h>

#include "ServerSide.h"

#define BUFFSIZE 256
/* Global Variables */
const char *Program = NULL;
struct sockaddr_in ServerAddress;
/*GtkWidget  *requestbutton;*/

int NewVal;
gboolean DrawAreaExposed(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GtkWidget *DrawArea;
	Map *map = data;
	cairo_t *cr;
	int i, j, k, l;

	printf("%s: DrawAreaExposed callback starting...\n", Program);

	DrawArea = widget;

	cr = gdk_cairo_create(DrawArea->window);

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, 149, 69, 11*map->col+1, 11*map->row+1);
	cairo_fill(cr);

			
	for(i=0; i<=map->col; i=i+1.0) {
		for(j=0; j<=map->row; j=j+1.0) {
			if(map->streets[i][j] == 0) {
				cairo_rectangle(cr, (11.0*(i+1)-6.0)+145, (11.0*(j+1)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
				cairo_fill(cr);
			}
			else if(map->streets[i][j] == 1) {
				cairo_rectangle(cr, (11.0*(1+i)-6.0)+145, (11.0*(1+j)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
				cairo_fill(cr);
			}
			else if(map->streets[i][j] == 2) {
				cairo_rectangle(cr, (11.0*(1+i)-6.0)+145, (11.0*(1+j)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
				cairo_fill(cr);
			}
			else if(map->streets[i][j] == 3) {
				cairo_rectangle(cr, (11.0*(1+i)-6.0)+145, (11.0*(1+j)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
				cairo_fill(cr);
			}
			else if(map->streets[i][j] == 4) {
				cairo_rectangle(cr, (11.0*(1+i)-6.0)+145, (11.0*(1+j)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 1.0, 0.0, 1.0);
				cairo_fill(cr);
			}
		}
	} 
	cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
	cairo_select_font_face(cr, "Times New Roman", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 10.0);
/*	cairo_rotate(cr, 0.7854);*/
	for(k=1.0; k<=map->col; k=k+1) {
		cairo_move_to(cr, 11*k-6+150, 60);
		cairo_rotate(cr, -1.571); 
		cairo_show_text(cr, map->colNames[k-1]);
		cairo_rotate(cr, 1.571);
	}

	for(l=1.0; l<=map->row; l=l+1) {
		cairo_move_to(cr, 10, 11*l-6+75);
		cairo_show_text(cr, map->rowNames[l-1]);
	}
	cairo_set_font_size(cr, 12.5);
	cairo_move_to(cr, 10, 10);
	cairo_show_text(cr, map->title);
	/* cairo_stroke_preserve(cr); */
	/*cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_fill(cr);*/
	return TRUE;
}

void FatalError( const char *ErrorMsg) /* notifies user of error diagnostics and aborts */
{
	fputs(Program, stderr);
	fputs(": ", stderr);
	perror(ErrorMsg);
	fputs(": Exiting!]n", stderr);
	exit(20);
} /*End of Fatal Error*/

char *Talk2Server(		/* communicate with the server */
	const char *Message,
	char *RecvBuf)
{
    int n;
    int SocketFD;

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {   FatalError("socket creation failed");
    }
#ifdef DEBUG
    printf("%s: Connecting to the server at port %d...\n",
		Program, ntohs(ServerAddress.sin_port));
#endif
    if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
		sizeof(struct sockaddr_in)) < 0)
    {   FatalError("connecting to server failed");
    }
#ifdef DEBUG
    printf("%s: Sending message '%s'...\n", Program, Message);
#endif
    n = write(SocketFD, Message, strlen(Message));
    if (n < 0)
    {   FatalError("writing to socket failed");
    }
#ifdef DEBUG
    printf("%s: Waiting for response...\n", Program);
#endif
    n = read(SocketFD, RecvBuf, BUFFSIZE-1);
    if (n < 0) 
    {   FatalError("reading from socket failed");
    }
    RecvBuf[n] = 0;
#ifdef DEBUG
    printf("%s: Received response: %s\n", Program, RecvBuf);
    printf("%s: Closing the connection...\n", Program);
#endif
    close(SocketFD);
    return(RecvBuf);
    
}
static void destroy (GtkWidget *widget, gpointer data )
{
	gtk_main_quit ();
}


void ShutdownServer( GtkWidget *Widget, gpointer Data) /*Asks server to shutdown*/
{
	char RecvBuf[256];
	const char *Response;
	printf("%s: ShutdownServer callback starting...\n", Program);
	Response = Talk2Server("SHUTDOWN", RecvBuf);
	if (0 == strcmp(Response, "OK SHUTDOWN")) {
		gtk_main_quit();
	}
	else {
	}
	printf("%s: ShutdownServer callback done.\n",Program);
}  /* End of ShutdownServer */
static void init_window(GtkWidget *window)
{
	gtk_window_set_title(GTK_WINDOW(window),"Taxi Client"); /*Sets window title*/
	gtk_window_set_default_size(GTK_WINDOW(window),400,400); /*Sets default size for window*/
	g_signal_connect(window,"destroy", G_CALLBACK(destroy), NULL); /*Ends application when close button is clicked*/
}

/* Attempt at creating a function to callback multiple arguments / entries */
static void callback(GtkWidget *widget, GtkWidget **entry)
{
	GtkWidget *entry_ptr_1 = entry[0];
	GtkWidget *entry_ptr_2 = entry[1];

	const gchar *e1, *e2;

	e1 = gtk_entry_get_text(GTK_ENTRY(entry_ptr_1));
	e2 = gtk_entry_get_text(GTK_ENTRY(entry_ptr_2));
/*	printf("Your current location is %s\n", e1);
	printf("Your desired destination is %s\n", e2);*/
} 
static void GetEntries(GtkWidget *widget, GtkWidget **entry)
{
	char RecvBuf[256];
	char *E1;
	char *E2;
	char *pick;
	char *drop;
	char *pick1, *pick2;
	char *drop1, *drop2;
	
	NewVal=0;
	pick = malloc(sizeof(char)*4);
	drop = malloc(sizeof(char)*4);
	drop[3] = '\0';
	pick[3] = '\0';
	pick1 = malloc(sizeof(char));
	pick2 = malloc(sizeof(char));
	
	drop1 = malloc(sizeof(char));
	drop2 = malloc(sizeof(char));

	const char *Response;
	GtkWidget *entry_ptr_1 = entry[0];
	GtkWidget *entry_ptr_2 = entry[1];
	const gchar *e1, *e2;
	char comm[256] = "REQUEST_TAXI ";
	
	int i, j, end;
	
	e1 = gtk_entry_get_text(GTK_ENTRY(entry_ptr_1));
	e2 = gtk_entry_get_text(GTK_ENTRY(entry_ptr_2));
	E1 = g_strdup(e1);

	E2 = g_strdup(e2);

	

	pick1 = strtok(E1, "-");
	pick2 = strtok(NULL, "-"); 

	drop1 = strtok(E2, "-");
	drop2 = strtok(NULL, "-");
	
	char *filename = "NewIrvine.map";
	Map *map = newMap();
	assert(loadMap(filename, map) == 0);
	/* Converting to ascii for pickup */
	char *s, *b, *loc;
	loc = malloc(sizeof(char));
	int check1 = 0;
	int check2 = 0;
	int check3 = 0;
	int check4 = 0;
	for(i = 0; i < map->row; i++){
		
		for(j = 0; j < map->col; j++){	


			if(strcmp(pick1, map->rowNames[i]) == 0){
				pick[0] = i + 65;
				check1 =1;	
					
			}
			else if(strcmp(pick1, map->colNames[j]) == 0){
				if(j > 9){/* FIX IT TO WHERE IT WORKS FOR 10TH STREET */
					end = j + 1;
					sprintf(s , "%d", (end/10) + 48);
					sprintf(b , "%d", (end%10) + 48);
					pick[1] = s;
					pick[2] = b;
				}
				else{
					pick[1] = j + 49;
					pick[2] = '\0';
				}
				check2=1;
			}
			if(strcmp(pick2, map->rowNames[i]) == 0){
				pick[0] = i + 65;	
				check1 = 1;		
			}
			else if(strcmp(pick2, map->colNames[j]) == 0){
				if(j > 9){
					end = j + 1;
					s = (end/10) + 48;
					b = (end%10) + 48;
					pick[1] = s;
					pick[2] = b;
				}
				else{
					pick[1] = j + 49;
					pick[2] = NULL;
				}
				check2 = 1;
			}
		}
	}
	/* Converting to ascii for drop */
	for(i = 0; i < map->row; i++){
		
		for(j = 0; j < map->col; j++){	


			if(strcmp(drop1, map->rowNames[i]) == 0){
				drop[0] = i + 65;	
				check3 =1;		
			}
			else if(strcmp(drop1, map->colNames[j]) == 0){
				if(j > 9){
					end = j + 1;
					s = (end/10)+ 48;
					b = (end%10)+ 48;
					drop[1] = s;
					drop[2] = b;
				}
				else{
					drop[1] = j;
					drop[2] = NULL;
				}
				check4=1;
			}
			if(strcmp(drop2, map->rowNames[i]) == 0){
				drop[0] = i + 65;	
				check3=1;		
			}
			else if(strcmp(drop2, map->colNames[j]) == 0){
				if(j > 9){
					end = j + 1;
					s = (end/10) + 48;
					b = (end%10) + 48;
					drop[1] = s;
					drop[2] = b;
				}
				else{
					drop[1] = j + 49;
					drop[2] = NULL;
				}
				check4=1;
			}
		}
	}
	if(check1==1 && check2==1 && check3==1 && check4==1){
		int pick_x=0, pick_y=0;
		int drop_x=0, drop_y=0;
		  
		pick_y = pick[0] - 65;
		for(i = 1; pick[i] != '\0'; i++){
			pick_x = pick_x * 10 + pick[i] - '0';
		}
		/*pick_x = pick_x - 1;*/
		 
		drop_y = drop[0] - 65;
		for(i = 1; drop[i] != '\0'; i++){
			drop_x = drop_x * 10 + drop[i] - '0';
		}
		/*drop_x = drop_x -1;*/ 

		BuildArrayObstacle(map);
		/*printf("%d", map->streets[10][15]); This is a 1 on the int array Pale Loop 10th Street */ 
		
		if(map->streets[pick_x][pick_y] == 1){
			for(i = 0; i < map->landmarks->size; i++){

				Landmark *landmark = (char *) map->landmarks->items + sizeof(Landmark) * i;
				if(landmark->topLeft.x<=pick_y && landmark->topLeft.y<= pick_x && landmark->botRight.x>=pick_y && landmark->botRight.x>= pick_y)
				{
					pick_x = landmark->mark.y + 1;
					pick_y = landmark->mark.x;
				}
			}
			pick[0] = pick_y + 65;
			if(pick_x>9){
				 s = (pick_x/10) + 48;
				 b = (pick_x%10) + 48;
				 pick[1] = s;
				 pick[2] = b;

			}
			else{
				pick[1] = pick_x + 48;
				pick[2] = NULL;
			}

		}	
		if(map->streets[drop_x][drop_y] == 1){
			for(i = 0; i < map->landmarks->size; i++){


				Landmark *landmark = (char *) map->landmarks->items + sizeof(Landmark) * i;
				if(landmark->topLeft.y<=drop_x && landmark->topLeft.x<= drop_y && landmark->botRight.y>=drop_x && landmark->botRight.x>= drop_y)
				{
					drop_x = landmark->mark.y + 1;
					drop_y = landmark->mark.x;
				}
			}
			drop[0] = drop_y + 65;
			if(drop_x>9){
				 s = (drop_x/10) + 48;
				 b = (drop_x%10) + 48;
				 drop[1] = s;
				 drop[2] = b;
			 
			} 
			else{
				drop[1] = drop_x +48;
				drop[2] = NULL;
			}
		} 
		char TO[6] = " TO ";
		strcat(comm, pick);
		strcat(comm, TO);
		strcat(comm, drop);
		printf("%s!\n\n", comm);
		deleteMap(map);
		NewVal=1;	
		printf("\n\n\n%d", NewVal);
		Response = Talk2Server(comm, RecvBuf);
		printf("%s", Response);
		gtk_label_set_text(entry[2], Response);
	}
	else{
		deleteMap(map);
		/*strcpy(Response, "Please Enter a valid location");*/
		printf("Please Enter a valid street\n");
		gtk_label_set_text(entry[2], "Please Enter a valid location");
	}	
}
void ChangeButton(GtkWidget *widget){
	gtk_button_set_label(widget, "Confirm");

	}
void ChangeButton1(GtkWidget *widget){
        gtk_button_set_label(widget, "Request Location");

        }

/*void ConfirmLocation(){
        char RecvBuf[256];
        const char *Response;
	char NewComm[256]= "CONFIRM";
	if(NewVal==1){
	printf("\n\n\n%s@@@@\n\n", NewComm);
	Response = Talk2Server(NewComm, RecvBuf);
	gtk_label_set_text(entry[2], Response);
        }
	else{
		printf("\n Please Request Taxi before confirmation");
	}
	}*/
/*void RequestLocation(){
	char RecvBuf[256];
} */
/*void getMessage(GtkWidget *widget, gpointer data){
	assert data;
	char* message;
	message = GetEntries(
	char *Response = Talk2Server(
}*/
/*static void requestlabel (GtkWidget *widget, gpointer data)
{
	const gchar *newlabel;
	newlabel = Response;
	gtk_label_set_label(data, newlabel);
}*/
int main(int argc, char *argv[])
{


  	int PortNo;
    	struct hostent *Server;

    	Program = argv[0];
#ifdef DEBUG
    	printf("%s: Starting...\n", argv[0]);
#endif
    	if (argc < 3)
    	{   	fprintf(stderr, "Usage: %s hostname port\n", Program);
		exit(10);
    	}
    	Server = gethostbyname(argv[1]);
	if (Server == NULL)
    	{  		fprintf(stderr, "%s: no such host named '%s'\n", Program, argv[1]);
        	exit(10);
	}
	PortNo = atoi(argv[2]);
	if (PortNo <= 2000)
    	{   	fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		Program, PortNo);
        	exit(10);
	}
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(PortNo);
	ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0]; 
/* Loading map ********************************************************************/

	char *filename = "NewIrvine.map";
	Map *map = newMap();
	loadMap(filename, map);
	BuildArrayObstacle(map);
	


	GtkWidget *window, *DrawArea, *label1, *label2, *label3, **entry, *entry1, *entry2, *frame, *requestbutton, *requestlocbutton, *VBox,*HBox, *HButtons, *quitbutton, *shutdownbutton, *confirmbutton;
	entry = malloc(1*sizeof(GtkWidget));

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	init_window(window); 
    
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); /*Sets window in center of screen*/

	gtk_container_set_border_width (GTK_CONTAINER(window), 10); /*Sets border width */

	VBox = gtk_vbox_new(FALSE,10);
	gtk_container_add(GTK_CONTAINER(window), VBox);

	HBox = gtk_hbox_new(FALSE,10);
	gtk_widget_set_size_request(HBox, 650, 400);
	gtk_container_add(GTK_CONTAINER(VBox), HBox);

	
	DrawArea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(HBox), DrawArea);

	label1 = gtk_label_new ("Enter pick up location: (streetname)-(streetname)");
	gtk_container_add(GTK_CONTAINER(VBox), label1);
	
	entry1 = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry1),0);
	gtk_container_add(GTK_CONTAINER(VBox), entry1);

	label2 = gtk_label_new ("Enter desired destination: (streetname)-(streetname)");
	gtk_container_add(GTK_CONTAINER(VBox), label2);

	entry2 = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry2),0);
	gtk_container_add(GTK_CONTAINER(VBox), entry2);

	requestbutton = gtk_button_new_with_label("Request Ride");
	gtk_container_add(GTK_CONTAINER(VBox), requestbutton);

        confirmbutton = gtk_button_new_with_label("Confirm");
        gtk_container_add(GTK_CONTAINER(VBox), confirmbutton);

	requestlocbutton = gtk_button_new_with_label("Request Location");
	gtk_container_add(GTK_CONTAINER(VBox), requestlocbutton);

	frame = gtk_frame_new("Ride Status:");
	gtk_container_add(GTK_CONTAINER(VBox), frame);
	label3 = gtk_label_new("<No Request Sent>");
	gtk_container_add(GTK_CONTAINER(frame), label3);

	HButtons = gtk_hbutton_box_new();
	gtk_container_add(GTK_CONTAINER(VBox), HButtons);
	quitbutton = gtk_button_new_with_label("Quit Client");
	gtk_container_add(GTK_CONTAINER(HButtons), quitbutton);
	shutdownbutton = gtk_button_new_with_label("Shutdown Server");
	gtk_container_add(GTK_CONTAINER(HButtons), shutdownbutton);
	
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

		/*strcat(Entry1, Entry2);*/
	g_signal_connect(DrawArea, "expose_event", G_CALLBACK(DrawAreaExposed), map);
			
	g_signal_connect(requestbutton, "clicked", G_CALLBACK(GetEntries), entry);
	
	
/*	g_signal_connect(requestbutton, "clicked", G_CALLBACK(callentry1), entry1);	*/
   /*     g_signal_connect(confirmbutton, "clicked", G_CALLBACK(ConfirmLocation), NULL);*/
	 
	
/*	g_signal_connect(requestbutton, "clicked", G_CALLBACK(callentry2), entry2); */

	g_signal_connect(requestbutton, "clicked", G_CALLBACK(callback), entry);

/*        g_signal_connect(requestbutton, "clicked", G_CALLBACK(ChangeButton), confirmbutton);*/

/*	g_signal_connect(requestlocbutton, "clicked", G_CALLBACK(RequestLocation), NULL);*/	
	
	entry[0] = entry1;
	entry[1] = entry2;
	entry[2] = label3;

/*	g_signal_connect(requestbutton, "clicked", G_CALLBACK(requestlabel), label3); */

	/*if(NewVal==1){
	        printf("It worked");
		g_signal_connect(requestbutton, "clicked", G_CALLBACK(ConfirmLocation), NULL);

	}*/
        g_signal_connect(DrawArea, "expose_event", G_CALLBACK(DrawAreaExposed), map);

	g_signal_connect_swapped(quitbutton, "clicked", G_CALLBACK(gtk_widget_destroy), window);

	g_signal_connect_swapped(shutdownbutton,"clicked", G_CALLBACK(ShutdownServer), NULL);
	
	g_signal_connect_swapped(shutdownbutton,"clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_widget_show_all(window);
        

	gtk_main();
	
		

	return 0;
}
