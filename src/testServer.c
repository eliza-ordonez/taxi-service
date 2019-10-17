#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <assert.h>
#include <gtk/gtk.h>
#include <time.h>
#include "ServerSide.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
/* #define DEBUG */	/* be verbose */

/*** type definitions ****************************************************/

typedef void (*ClientHandler)(int DataSocketFD);
typedef void (*TimeoutHandler)(void);

/*** global variables ****************************************************/

const char *Program	/* program name for descriptive diagnostics */
	= NULL;
int Shutdown		/* keep running until Shutdown == 1 */
	= 0;
char ClockBuffer[26]	/* current time in printable format */
	= "";
GtkWidget *map1 = NULL;
GtkWidget *redraw;
struct Map_t *map;
Taxi *taxi; 

int Confirm = 1;
int check = 0;

/***GUI Functions*********************************************************/

void drawback(GtkWidget *widget){
	if(check == 1){
		check = 0;
		gtk_widget_queue_draw(widget);
	}

}

gboolean DrawAreaExposed(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GtkWidget *DrawArea;
	gint Width, Height;
	Map *map1 = map;
	cairo_t *cr;
	int i, j, k, l;
	int m, n;
	int x = map1->col;
	int y = map1->row;
	drawback(widget);
	printf("%s: DrawAreaExposed callback starting...\n", Program);
	DrawArea = widget;
	redraw = widget;
	Width = 11.0*x+10;/*DrawArea->allocation.width;*//* 42;*/
	Height = 11*y+10;/*DrawArea->allocation.height;*//* 26;*/
	cr = gdk_cairo_create(DrawArea->window);

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, 149, 69, 11*map1->col+1, 11*map1->row+1);
	cairo_fill(cr);

		
	for(i=0; i<=map1->col; i=i+1.0) {
		for(j=0; j<=map1->row; j=j+1.0) {
			if(map1->streets[i][j] == 0) {
				cairo_rectangle(cr, (11.0*(i+1)-6.0)+145, (11.0*(j+1)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
				cairo_fill(cr);
			}
			else if(map1->streets[i][j] == 1) {
				cairo_rectangle(cr, (11.0*(i+1)-6.0)+145, (11.0*(j+1)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
				cairo_fill(cr);
			}
			else if(map1->streets[i][j] == 2) {
				cairo_rectangle(cr, (11.0*(i+1)-6.0)+145, (11.0*(j+1)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
				cairo_fill(cr);
			}
			else if(map1->streets[i][j] == 3) {
				cairo_rectangle(cr, (11.0*(i+1)-6.0)+145, (11.0*(j+1)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
				cairo_fill(cr);
			}
			else if(map1->streets[i][j] == 4) {
				cairo_rectangle(cr, (11.0*(i+1)-6.0)+145, (11.0*(j+1)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 1.0, 0.0, 1.0);
				cairo_fill(cr);
			}
			else if(map1->streets[i][j] == 5) {
				cairo_rectangle(cr, (11.0*(i+1)-6.0)+145, (11.0*(j+1)-6.0)+65, 10.0, 10.0);
				cairo_set_source_rgb(cr, 0.0, 1.0, 1.0);
				cairo_fill(cr);
			}
			else if(map1->streets[i][j] == 6) {
				cairo_set_line_width(cr, 1);
				cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
			/*	cairo_translate(cr, (11.0*(i+1)-6.0)+145, (11.0*(j+1)-6.0)+65);*/
				cairo_arc(cr, (11.0*(i+1)-6.0)+150, (11.0*(j+1)-6.0)+70, 5, 0, 2 * M_PI);
				cairo_stroke_preserve(cr);
				cairo_set_source_rgb(cr, 1.0, 1.0, 0.0);
				cairo_fill(cr);
			}
		}
	} 
	cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
	cairo_select_font_face(cr, "Times New Roman", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 10.0);
/*	cairo_rotate(cr, 0.7854);*/
	for(k=1.0; k<=map1->col; k=k+1) {
		cairo_move_to(cr, 11*k-6+150, 60);
		cairo_rotate(cr, -1.571); 
		cairo_show_text(cr, map1->colNames[k-1]);
		cairo_rotate(cr, 1.571);
	}

	for(l=1.0; l<=map1->row; l=l+1) {
		cairo_move_to(cr, 10, 11*l-6+75);
		cairo_show_text(cr, map1->rowNames[l-1]);
	}
	cairo_set_font_size(cr, 12.5);
	cairo_move_to(cr, 10, 10);
	cairo_show_text(cr, map1->title);
	/* cairo_stroke_preserve(cr); */
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_fill(cr);

/*	cairo_set_font_size(cr, 15);
 *		for(m=0.0; m<(map1->landmarks->size); m=m+1) {
 *				Landmark *landmark = (char *) map1->landmarks->items + sizeof(Landmark) * m;
 *						cairo_move_to(cr, (landmark->topLeft.x + landmark->botRight.x) / 2, (landmark->topLeft.y + landmark->botRight.y) / 2);   
 *								cairo_move_to(cr, 100, 100);
 *										cairo_show_text(cr, landmark->name[0]);
 *											} */

	cairo_destroy(cr);
	return TRUE;
}

void ShutdownClicked(GtkWidget *widget, gpointer data)
{
	Shutdown = 1;
	map1 = NULL;
}

void UpdateDistance(GtkWidget *widget, gpointer data)
{
	const gchar *test = "test distance";
	gtk_label_set_text(data, test);
}
void UpdateETA(GtkWidget *widget, gpointer data)
{
	const gchar *test = "test ETA";
	gtk_label_set_text(data, test);
}

void UpdateMoney(GtkWidget *widget, gpointer data)
{
	const gchar *test = "test $$$";
	gtk_label_set_text(data, test);
}
void UpdateWindow(void){
	while(gtk_events_pending()){
		gtk_main_iteration();
	}

}

GtkWidget *CreateWindow( int *argc, char **argv[], Map *map)
{
	GtkWidget *window, *HBox, *VBox, *DrawArea1, *ShutdownButton, *CashFrame, *CashLabel, *ETAframe, *ETAlabel, *DistanceFrame, *DistanceLabel, *ProfitFrame, *ProfitLabel, *testbutton;

	gtk_init(argc, argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), Program);
	gtk_window_set_default_size(GTK_WINDOW(window), 5.0*(3.0*map->col-1.0)+270, 5.0*(3.0*map->row-1.0)+50);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	HBox = gtk_hbox_new(FALSE, 0);
	gtk_widget_set_size_request(HBox, 5.0*(3.0*map->col-1.0)+100, 5.0*(3.0*map->row-1.0)+50);
	gtk_container_add(GTK_CONTAINER(window), HBox);

	VBox = gtk_vbox_new(TRUE, 0);
	gtk_widget_set_size_request(VBox, 150, 5.0*(3.0*map->row-1.0)+50);

	DrawArea1 = gtk_drawing_area_new();
	gtk_widget_set_size_request(DrawArea1, 700, 500); 
	gtk_container_add(GTK_CONTAINER(HBox), DrawArea1);

	gtk_container_add(GTK_CONTAINER(HBox), VBox);

	ProfitFrame = gtk_frame_new("Profit Earned($):");
	gtk_container_add(GTK_CONTAINER(VBox), ProfitFrame);

	ProfitLabel = gtk_label_new("No Profit Earned Yet");
	gtk_container_add(GTK_CONTAINER(ProfitFrame), ProfitLabel);

	CashFrame = gtk_frame_new("Driver Earned($):");
	gtk_container_add(GTK_CONTAINER(VBox), CashFrame);

	CashLabel = gtk_label_new("No Money Received Yet");
	gtk_container_add(GTK_CONTAINER(CashFrame), CashLabel);

	DistanceFrame = gtk_frame_new("Distance of Trip:");
	gtk_container_add(GTK_CONTAINER(VBox), DistanceFrame);

	DistanceLabel = gtk_label_new("No Ride Request Sent Yet");
	gtk_container_add(GTK_CONTAINER(DistanceFrame), DistanceLabel);

	ETAframe = gtk_frame_new("Estimated Time of Arrival:");
	gtk_container_add(GTK_CONTAINER(VBox), ETAframe);

	ETAlabel = gtk_label_new("No Ride Request Sent Yet");
	gtk_container_add(GTK_CONTAINER(ETAframe), ETAlabel);

	ShutdownButton = gtk_button_new_with_label("Shutdown Server and Quit");
	gtk_container_add(GTK_CONTAINER(VBox), ShutdownButton); 

	testbutton = gtk_button_new_with_label("Click me to test labels");
	gtk_container_add(GTK_CONTAINER(VBox), testbutton);
	
	gtk_widget_show_all(window);
	
	g_signal_connect(DrawArea1, "expose_event", G_CALLBACK(DrawAreaExposed), map);


	g_signal_connect(testbutton, "clicked", G_CALLBACK(UpdateETA), ETAlabel);

	g_signal_connect(testbutton, "clicked", G_CALLBACK(UpdateMoney), CashLabel);

	g_signal_connect(testbutton, "clicked", G_CALLBACK(UpdateMoney), ProfitLabel);

	g_signal_connect(testbutton, "clicked", G_CALLBACK(UpdateDistance), DistanceLabel);

	g_signal_connect(window, "destroy", G_CALLBACK(ShutdownClicked), NULL);

	g_signal_connect(ShutdownButton, "clicked", G_CALLBACK(ShutdownClicked), NULL); 

	return(window);
}
/*** global functions ****************************************************/

void FatalError(		/* print error diagnostics and abort */
	const char *ErrorMsg)
{
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!\n", stderr);
    exit(20);
} /* end of FatalError */

int MakeServerSocket(		/* create a socket on this server */
	uint16_t PortNo)
{
    int ServSocketFD;
    struct sockaddr_in ServSocketName;

    /* create the socket */
    ServSocketFD = socket(PF_INET, SOCK_STREAM, 0);
    if (ServSocketFD < 0)
    {   FatalError("service socket creation failed");
    }
    /* bind the socket to this server */
    ServSocketName.sin_family = AF_INET;
    ServSocketName.sin_port = htons(PortNo);
    ServSocketName.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ServSocketFD, (struct sockaddr*)&ServSocketName,
		sizeof(ServSocketName)) < 0)
    {   FatalError("binding the server to a socket failed");
    }
    /* start listening to this socket */
    if (listen(ServSocketFD, 5) < 0)	/* max 5 clients in backlog */
    {   FatalError("listening on socket failed");
    }
    return ServSocketFD;
} /* end of MakeServerSocket */

/*void PrintCurrentTime(void)	
 * {
 *     time_t CurrentTime; 
 *         char   *TimeString;
 *             char   Wheel,
 *             	   *WheelChars = "|/-\\";
 *             	       static int WheelIndex = 0;
 *
 *             	           CurrentTime = time(NULL);	
 *             	               TimeString = ctime(&CurrentTime);	
 *             	                   strncpy(ClockBuffer, TimeString, 25);
 *             	                       ClockBuffer[24] = 0;	
 *             	                           WheelIndex = (WheelIndex+1) % 4;
 *             	                               Wheel = WheelChars[WheelIndex];
 *             	                                   printf("\rClock: %s %c",	
 *             	                                   	ClockBuffer, Wheel);	
 *             	                                   	    fflush(stdout);
 *             	                                   	    } */

void ProcessRequest()
{
    int  l, n;
    char RecvBuf1[256];
	char RecvBuf[256]= "REQUEST_TAXI J7 TO X27";	/* message buffer for receiving a message */
    char SendBuf[256];	/* message buffer for sending a response */
    char *pick;
    char *drop;
/*    char comm[256];
 *        char pick[2];
 *            char drop[2];
 *                char locate[6];*/
   
    RecvBuf[n] = 0;
    int i;
   /* j = 0;
 *     end = 0;
 *     	for(i = 0; i < strlen(RecvBuf) + 1; i++){
 *     			    comm[i] = RecvBuf[i];
 *     			    		    end++;
 *     			    		    		   if(RecvBuf[i+1] == ' '){
 *     			    		    		   			  break;
 *     			    		    		   			  		   }
 *     			    		    		   			  		   	}*/
#ifdef DEBUG
    printf("%s: Received message: %s\n", Program, RecvBuf);
#endif
   /* 	char *filename = "NewIrvine.map";
 *  		Map *map = newMap();
 *  			loadMap(filename, map);
 *  				BuildArrayObstacle(map);
 *  					PrintServerMap(map);*/

    if (RecvBuf[8] == 'T'){
/*	    pick = malloc(3*sizeof(char));
 *	    	    drop = malloc(3*sizeof(char));
 *	    	    	    pick[2] = '\0';
 *	    	    	    	    drop[2] = '\0';
 *
 *	    	    	    	    	    for(i = 8; i < 10; i++){
 *	    	    	    	    	    		   pick[i - 8] = RecvBuf[i]; 
 *	    	    	    	    	    		   		   drop[i - 8] = RecvBuf[i + 2];
 *	    	    	    	    	    		   		   	    } */
		BuildArrayObstacle(map);
	    Client *client1 = CreateClient();
	    int pick_x = 0, pick_y = 0;
	    int drop_x = 0, drop_y = 0;
	    strtok(RecvBuf, " ");
	    pick = strtok(NULL, " ");
	    strtok(NULL, " ");
	    drop = strtok(NULL, " ");

	    /* The x and y values for the pickup */
	    pick_y = pick[0] - 65;
	    for(i = 1; pick[i] != '\0'; i++){
		    pick_x = pick_x * 10 + pick[i] - '0';
	    }
	    pick_x = pick_x - 1;
	    /* The x and y values for the dropoff */
	    drop_y = drop[0] - 65;
	    for(i = 1; drop[i] != '\0'; i++){
		    drop_x = drop_x * 10 + drop[i] - '0';
	    }
	    drop_x = drop_x - 1;	    
	    client1->x = pick_x;
	    client1->y = pick_y;
	    client1->nx = drop_x;
	    client1->ny = drop_y;
		int j, k;
/*	    taxi = RequestRide(client1, map);*/
/*	    BuildArrayObstacle(map);*/

	    /*BuildArrayObstacle(map);*/
	    taxi = RequestRide(client1, map);
	    taxi->status = 5;

	    /*for(i = 0; i < map->row; i++){
 * 		    for(j = 0; j < map->col; j++){
 * 		    			    printf("%d", map->streets[j][i]);
 * 		    			    		    }
 * 		    			    		    		    printf("\n");
 * 		    			    		    		    	    }*/

	    int time, blocks = 0;
	    blocks += CalculateRoute(taxi->x, taxi->y, pick_x, pick_y, map, taxi->status);
	    blocks += CalculateRoute(pick_x, pick_y, client1->nx, client1->ny,map, taxi->status+1);
	    TaxiStand *BestStand;
	    taxi->x = client1->nx;
	    taxi->y = client1->ny;
	    BestStand = ReturnTaxi(taxi, map);
	    blocks += CalculateRoute(taxi->x, taxi->y, BestStand->mark.y, BestStand->mark.x, map, taxi->status);
	    PrintServerMap(map);
	    
	    time = eta(blocks);
/*	    drawback(redraw);*/
	    
		PrintServerMap(map);
		char *time1;
		char *s, *b;
		time1 = malloc(sizeof(char)*3);
		time1[2] = '\0';
		if(time > 9){
			time = time + 1;
			s = (time/10)+ 48;
			b = (time%10)+ 48;
			time1[0] = s;
			time1[1] = b;
		}
		else{
			time1[0] = time;
			time1[1] = NULL; 
		}
		if(time){
			check = 1;
		}	
	    printf("REQUESTED PICK UP: %s DROP OFF: %s\n", pick, drop);	    
	    strncpy(SendBuf, "TAXI REQUEST CONFIRMED, ETA: ", sizeof(SendBuf)-1);
	    SendBuf[sizeof(SendBuf)-1] = 0;
	    strncat(SendBuf, time1, sizeof(SendBuf)-1-strlen(SendBuf));
	    strncat(SendBuf, " minutes", sizeof(SendBuf)-1-strlen(SendBuf));
}

	if(RecvBuf[0]=='C'){
		int blocks;
		int time;
	    int l, n;
	    l = strlen(SendBuf);
	    int msec = 0, trigger = 10;
	    clock_t before = clock();
            blocks = DropOff(taxi, map);
            time = eta(blocks);
            blocks = ReturnTaxi(taxi, map);
            time = eta(blocks);

	map->streets[taxi->x][taxi->y] = 6;
	 /* while(taxi->x != pick_x || taxi->y != pick_y){
 * 	  if(map->streets[taxi->x+1][taxi->y] == 5){*/
	/*	map->streets[taxi->x][taxi->y] = 0;*/

	/*	  taxi->x++;
 *	  	  	map->streets[taxi->x][taxi->y] = 6;
 *	  	  			drawback(redraw);
 *
 *	  	  				  } 
 *	  	  				  	  else if (map->streets[taxi->x-1][taxi->y] == 5){*/
	/*	  map->streets[taxi->x][taxi->y] = 0;
 *
 *	  		  taxi->x--;
 *	  		  		  map->streets[taxi->x][taxi->y] = 6;
 *	  		  		  		drawback(redraw);
 *
 *	  		  		  			  }
 *	  		  		  			  	  else if (map->streets[taxi->x][taxi->y+1] == 5){
 *	  		  		  			  	  	   map->streets[taxi->x][taxi->y] = 0;
 *
 *	  		  		  			  	  	   		  taxi->y++;
 *	  		  		  			  	  	   		  		  map->streets[taxi->x][taxi->y] = 6;
 *	  		  		  			  	  	   		  		  	  	drawback(redraw);
 *
 *	  		  		  			  	  	   		  		  	  		  }
 *	  		  		  			  	  	   		  		  	  		  	  else if (map->streets[taxi->x][taxi->y-1] == 5){
 *	  		  		  			  	  	   		  		  	  		  	  		 map->streets[taxi->x][taxi->y] = 0;*/

	/*	taxi->y--;
 *		  	map->streets[taxi->x][taxi->y] = 6;
 *		  			drawback(redraw);
 *		  			  
 *		  			  	 }
 *		  			  	 	  drawback(redraw);
 *		  			  	 	  	  fflush(stdout);
 *		  			  	 	  	  	  sleep(1);
 *		  			  	 	  	  	  	  } */
	
/*	while(taxi->x != pick_x || taxi->y != pick_y){
 *
 *		MoveTaxi(taxi, pick_x, pick_y, map);
 *			drawback(redraw);*/
 /* }*/
}
/*	    deleteMap(map1);*/
    
    else if (0 == strcmp(RecvBuf, "SHUTDOWN"))
    {   Shutdown = 1;
	strncpy(SendBuf, "OK SHUTDOWN", sizeof(SendBuf)-1);
	SendBuf[sizeof(SendBuf)-1] = 0;
    }
    else
    {   strncpy(SendBuf, "ERROR unknown command ", sizeof(SendBuf)-1);
	SendBuf[sizeof(SendBuf)-1] = 0;
	strncat(SendBuf, RecvBuf, sizeof(SendBuf)-1-strlen(SendBuf));
    }
    l = strlen(SendBuf);
#ifdef DEBUG
    printf("%s: Sending response: %s.\n", Program, SendBuf);
#endif
    if (n < 0)
    {   FatalError("writing to data socket failed");
    }
	drawback(redraw);
} /* end of ProcessRequest */

void ServerMainLoop(		/* simple server main loop */
	int ServSocketFD,		/* server socket to wait on */
	void ProcessRequest(),	/* client handler to call */
		/* timeout handler to call */
	Map *map)			/* timeout in micro seconds */
{
    	/* client address we connect with */
    	/* socket file descriptors ready to read from */
    struct timeval TimeVal;
    int res, i;

    	/* server socket is active */
    int j;
    int check = 0;
    check++;
    printf("%d\n\n\n\n", check);
int v = 0;
while(1){  
/*	UpdateWindow();*/
	drawback(redraw);

	UpdateWindow();

/*	map->streets[0][0] = 5;
 *		for(j = 0; j < 15; j++){ 
 *				map->streets[j][0] = 5;
 *					}*/
/*ReadFDs = ActiveFDs;*/
	/*TimeVal.tv_sec  = Timeout / 1000000;*/	/* seconds */
	/*TimeVal.tv_usec = Timeout % 1000000;	*//* microseconds */
	/* block until input arrives on active sockets or until timeout */
/*	res = select(FD_SETSIZE, &ReadFDs, NULL, NULL, &TimeVal);
	if (res < 0)
	{   FatalError("wait for input or timeout (select) failed");
	}
	if (res == 0)*/	/* timeout occurred */
	
/*#ifdef DEBUG
	    printf("%s: Handling timeout...\n", Program);
#endif
	
	}
	else*/		/* some FDs have data ready to read */
/*	{   for(i=0; i<FD_SETSIZE; i++)
	    {   if (FD_ISSET(i, &ReadFDs))
		{   
			if (i == ServSocketFD)
		    {*/	/* connection request on server socket */
/*#ifdef DEBUG
			printf("%s: Accepting new client %d...\n", Program, i);
#endif
			ClientLen = sizeof(ClientAddress);
			DataSocketFD = accept(ServSocketFD,
				(struct sockaddr*)&ClientAddress, &ClientLen);

			if (DataSocketFD < 0)
			{   FatalError("data socket creation (accept) failed");
			}
#ifdef DEBUG
			printf("%s: Client %d connected from %s:%hu.\n",
				Program, i,
				inet_ntoa(ClientAddress.sin_addr),
				ntohs(ClientAddress.sin_port));
#endif
			FD_SET(DataSocketFD, &ActiveFDs);
		    }
		    else*/
		       /* active communication with a client */
/*#ifdef DEBUG
			printf("%s: Dealing with client %d...\n", Program, i);
#endif
			HandleClient();
      fflush(stdout);
#ifdef DEBUG
			printf("%s: Closing client %d connection.\n", Program, i);
#endif
			
		    }
		}
	    }
	}*/
	if(v==0){
	ProcessRequest();
	v++;
	}
}    
} /* end of ServerMainLoop */

/*** main function *******************************************************/

int main(int argc, char *argv[])
{
    int ServSocketFD;	/* socket file descriptor for service */
    int PortNo;		/* port number */
	GtkWidget *window;

    Program = argv[0];	/* publish program name (for diagnostics) */
#ifdef DEBUG
    printf("%s: Starting...\n", Program);
#endif
    if (argc < 2)
    {   fprintf(stderr, "Usage: %s port\n", Program);
	exit(10);
    }
    PortNo = atoi(argv[1]);	/* get the port number */
    if (PortNo <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		Program, PortNo);
        exit(10);
    }
#ifdef DEBUG
    printf("%s: Creating the server socket...\n", Program);
#endif
    ServSocketFD = MakeServerSocket(PortNo);
   /* printf("%s: Providing current time at port %d...\n", Program, PortNo);
 *     */
	char *filename = "NewIrvine.map";
	map = newMap();
	loadMap(filename, map);
	BuildArrayObstacle(map);
	PrintServerMap(map);

	printf("\n");	

	printf("%s: Creating the Server Window...\n", Program);
	window = CreateWindow(&argc, &argv, map);
	if (!window) {
		fprintf(stderr, "%s: cannot create GUI window\n", Program);
		exit(10);
	}
    printf("Coming for that CABooty Taxi Service\n");
    ServerMainLoop(ServSocketFD, ProcessRequest,
			 map);
    printf("\n%s: Shutting down.\n", Program);
    close(ServSocketFD);
    return 0;
}

