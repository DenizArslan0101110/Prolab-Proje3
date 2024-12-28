#include "Parse.h"
#include "Graph.h"
#include "Registers.h"
#include "Tree.h"
#include "rlgl.h"
#include "raymath.h"


struct Queue* queue = NULL;
int x_pos = 700;
int y_pos = 300;

void DrawBSTInOrder(struct BSDNode* root,int LeftOrRight,int previusX,int previusY,struct Author* data_list,Font myFont);

int Register1(struct Graph* main_graph, struct Author* data_list, int start, int finish, int is_simple,struct Position* position)
{
    int* dist = (int*)malloc(main_graph->vertice_number * sizeof(int));
    int* prev = (int*)malloc(main_graph->vertice_number * sizeof(int));
    queue = Dijkstra(main_graph, start, dist, prev, is_simple,position);
    if(!is_simple)
    {
        for(int i=0; i<main_graph->vertice_number ;i++)
        {
            write(1,"\x1b[0m",4);
            if(i==start) {write(1, "\x1b[44mSTRT: ", 11); goto PRINTING;}
            else if(i==finish) {write(1, "\x1b[44mFNSH: ", 11);goto PRINTING;}
            else if(dist[i]<INT_MAX) {_INF goto PRINTING;}
            else goto NOTPRINTING;
            PRINTING:
                write(1,"node_id: ",9);
                char iii[10]; int leng=snprintf(iii,sizeof(iii),"%d",i); write(1, iii, leng); snprintf(iii, 10-leng, "          "); write(1,iii,10-leng); write(1,"previous_node: ",15);
                char prevus[10]; int lengg=snprintf(prevus,sizeof(prevus),"%d",prev[i]); write(1, prevus, lengg); snprintf(prevus, 10-lengg, "          "); write(1,prevus,10-lengg); write(1,"distance: ",10);
                char distace[10]; int lenggg=snprintf(distace,sizeof(distace),"%d",dist[i]); write(1, distace, lenggg); snprintf(distace, 10-lenggg, "          "); write(1,distace,10-lenggg);
                write(1,"\x1b[0m\n",5);
            NOTPRINTING:;
        }
    }
    if(prev[finish]==-1 && !is_simple) {_WRN printf("Shortest path between %d and %d could not be found, nodes are not connected.\n",start,finish);}
    if(prev[finish]!=-1)
    {
            Font myFont = LoadFont("Comic.ttf");
            char Text[200];
            char Text2[200];
            sprintf(Text,"Shortest path between %d and %d has a distance of %d, and is as follows: \n", start, finish, dist[finish]);
            DrawTextEx(myFont,Text,(Vector2){x_pos,y_pos},30,1,WHITE);
            int current = finish;
            int xTimes = 0;
            sprintf(Text2,"%d",finish);

            DrawTextEx(myFont,Text2,(Vector2){x_pos+ 770,y_pos},30,1,WHITE);
            while(prev[current]!=-1)
            {

                char text2[20];
                current = prev[current];
                sprintf(text2," ---> %d",prev[current]);

                DrawTextEx(myFont,text2,(Vector2){x_pos + 800 + (78*xTimes),y_pos},30,1,WHITE);


                xTimes++;
            }

            y_pos += 30;

        _INF printf("Shortest path between %d and %d has a distance of %d, and is as follows: \n", start, finish, dist[finish]);
        printf("%d", finish);
        while(prev[current]!=-1)
        {
            printf(" ---> %d",prev[current]);
            current = prev[current];
        }
        printf("\n");
    }
    position[start].cf.color = YELLOW;
    position[finish].cf.color = YELLOW;
    return 1;
}

void Register2(struct Graph* main_graph, struct Author* data_list, int index,struct Position* position)
{
    struct Node* current = main_graph->list_of_adjacency_lists[index];
    int coauthor_no = 0;
    while (current != NULL)
    {
        coauthor_no++;
        current = current->next;
    }
    ///_DBG printf("Does the function at least start!?\n");
    struct Author* small_data_list = malloc((coauthor_no+3)*sizeof(struct Author));
    ///_DBG printf("Succes at making the struct.\n");
    small_data_list[0] = data_list[index];
    ///_DBG printf("Succes at getting temp value.\n");
    struct Node* temp = main_graph->list_of_adjacency_lists[index];
    int i=1;
    ///_DBG printf("Right before while loop\n");
    while(temp != NULL)
    {
        small_data_list[i] = data_list[temp->node_id];
        temp = temp->next; i++;
    }
// right at this point we made a small_data_list from all connected authors, this will be useful down below
    int paper_count_holder[coauthor_no+1];
    i=0;
    ///_DBG printf("Before finding papers.\n");
    while(small_data_list[i].name[0] != 0)
    {
        paper_count_holder[i] = FindFirstEmptyPaperSlot(small_data_list[i].papers);
        i++;
    }
    _INF printf("Sit tight, this can take a long time. (unlikely, but possible)\n");
    for(int aaa=0; aaa<FindFirstEmptyAuthorSlot(small_data_list)-1 ;aaa++)
    {
        AGAIN:
        if(paper_count_holder[aaa] < paper_count_holder[aaa+1])
        {
            int temporary = paper_count_holder[aaa+1];
            paper_count_holder[aaa+1] = paper_count_holder[aaa];
            paper_count_holder[aaa] = temporary;
            struct Author temporarius = small_data_list[aaa+1];
            small_data_list[aaa+1] = small_data_list[aaa];
            small_data_list[aaa] = temporarius;
            if(aaa>0) aaa--;
            goto AGAIN;
        }
    }
    ///_DBG printf("Before printing\n");
    for(int aaa=0; aaa<FindFirstEmptyAuthorSlot(small_data_list) ;aaa++)
    {
        _DBG printf("\"%s\" with %d papers was added to the queue.\n",small_data_list[aaa].name, paper_count_holder[aaa]);
    }

    for(int i = 0 ; i < coauthor_no+3 ; i++){
        for(int j = 0 ; j < main_graph->vertice_number-6; j++){
            if(strcmp(small_data_list[i].name,position[j].author.name) == 0){
                position[j].cf.color = GREEN;
            }
        }
    }



}

void Register3(struct Graph* main_graph, struct Author* data_list)
{

    if(queue == NULL){ _WRN printf("Queue does not exists, try Register 1 first.");}
    int list[512]; memset(list,-1,512*sizeof(int));
    int element = -1; int i=0;
    while((element = Dequeue(queue)) != -1 && i<512)
    {
        list[i] = element;
        i++;
    }
//  code chunk below destroys any duplicates in the list and sorts them into order (This is the worst code I have ever written but it works)
    for(int aaa=0; aaa<512 ;aaa++)
    {
        int spy = 0;
        if(list[aaa]!=-1) spy = list[aaa];
        for(int bbb=0; bbb<512 ;bbb++)
        {
            if(list[bbb]==spy && aaa!=bbb) list[bbb] = -1;
        }
    }
    for(int aaa=0; aaa<512 ;aaa++)
    {
        if(list[aaa]==-1 && list[aaa+1]!=-1)
        {
            list[aaa] = list[aaa+1];
            list[aaa+1] = -1;
            if(aaa>0) aaa--;
        }
    }
    i--;
    struct BSDNode* binary_search_tree = CreateBSDNode(list[i]); ///BUNA GORE ISLEM YAPILCAK

    i--;
    for(; i>0 ;i--)
    {

        Insert(binary_search_tree, list[i]);
    }
    /*
    _INF printf("Binary Search tree of the queue: \n");
    PrintInOrder(binary_search_tree);
    _INP printf("Which index would you like to delete off of the binary search tree?\n");
    int input = -1;
    scanf("%d", &input);
    Remove(binary_search_tree, input);
    _INF printf("Binary Search tree of the queue after deletion: \n");
    PrintInOrder(binary_search_tree);
    */


    Camera2D camera = {0};
    camera.target = (Vector2){0,0};
    camera.rotation = 0.0f;
    camera.offset = (Vector2){1000/2,800/2};
    camera.zoom = 1.0f;
    while(!WindowShouldClose())
    {
            Vector2 mousePosition = GetMousePosition();
            mousePosition.x = (mousePosition.x / camera.zoom) + (camera.target.x - (camera.offset.x / camera.zoom));
            mousePosition.y = (mousePosition.y / camera.zoom) + (camera.target.y - (camera.offset.y / camera.zoom));

            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                Vector2 delta = GetMouseDelta();
                delta = Vector2Scale(delta, -1.0f/camera.zoom);
                camera.target = Vector2Add(camera.target, delta);
            }
            Font myFont = LoadFont("Comic.ttf");
            if(IsKeyPressed(KEY_T)){
                    while(!IsKeyReleased(KEY_T)){BeginDrawing();EndDrawing(); }
                Remove(binary_search_tree,GetInput(myFont));
            }


            BeginDrawing();

            BeginMode2D(camera);
            ClearBackground(BLACK);
            DrawTextEx(myFont,"T'ye basarak cikartma islemi yapabilirsiniz !",(Vector2){camera.target.x-480,camera.target.y-400},40,1,WHITE);
            DrawCircle(700,150,20,GREEN);
            DrawBSTInOrder(binary_search_tree,-1,700,150,data_list,myFont);

            EndMode2D();
            EndDrawing();
    }
}

void Register4(struct Graph* main_graph, struct Author* data_list, int index,struct Position* position)
{

    if(data_list[index].orcid[0]!=0)
    {
        _WRN printf("Given author has no connections of depth \"2\" and as such, cant be calculated.\n");
        goto FUCKTHISIMOUT;
    }
    int to_visit[512]; memset(to_visit, -1, 512*sizeof(int));
    struct Node* current = main_graph->list_of_adjacency_lists[index];
    struct Node* current2 = NULL;
    int i=0;
    while (current != NULL)
    {
        current2 = main_graph->list_of_adjacency_lists[current->node_id];
            while (current2 != NULL)
            {
                to_visit[i] = current2->node_id; i++;
                current2 = current2->next;
            }
        current = current->next;
    }
    int yPosChanger = 0.0f;


    while(!WindowShouldClose()){
        yPosChanger -= ((float)GetMouseWheelMove()*50);
        x_pos = 340;
        y_pos = 200 + yPosChanger;
        BeginDrawing();
        ClearBackground(BLACK);
        for(i=0; to_visit[i] != -1 ;i++)
        {
            Register1(main_graph, data_list, index, to_visit[i], 1,position);
            if(to_visit[i] != -1) i++;
        }
        EndDrawing();
    }

    FUCKTHISIMOUT:;
}

int Register5(struct Graph* main_graph, struct Author* data_list, int index)
{
    struct Node* current = main_graph->list_of_adjacency_lists[index];
    int coauthor_no = 0;
    while (current != NULL)
    {
        coauthor_no++;
        current = current->next;
    }
    return coauthor_no;
}

int Register6(struct Graph* main_graph, struct Author* data_list)
{
    int most = -1, coop_no_max = 0, temp=0;
    char text[100];
    for(int i=0; i<main_graph->vertice_number ;i++)
    {
        temp = Register5(main_graph, data_list, i);
        if(temp>coop_no_max)
        {
            coop_no_max=temp;
            most=i;
        }
    }
    sprintf(text,"with %d cooperations, author's id: %d\n",coop_no_max, most);
    while(!WindowShouldClose()){
        Font myFont = LoadFont("Comic.ttf");
        BeginDrawing();
        if(IsKeyPressed(KEY_ENTER))
        {
            break;
        }
        DrawRectangle(1000/2,800/2 - 100,600,300,GRAY);
        DrawTextEx(myFont,text,(Vector2){1000/2 + 50,+800/2,500},40,1,WHITE);
        EndDrawing();
        }
        while(!IsKeyReleased(KEY_ENTER)){BeginDrawing();EndDrawing();}
    _DBG printf("With %d cooperations, author's id: %d\n",coop_no_max, most);
    return most;
}

int Register7(struct Graph* main_graph, struct Author* data_list, int start,struct Position* position)
{
    int* dist = (int*)malloc(main_graph->vertice_number * sizeof(int));
    int* prev = (int*)malloc(main_graph->vertice_number * sizeof(int));
    Dijkstra(main_graph, start, dist, prev, 1,position);
    for(int i=0; i<main_graph->vertice_number ;i++)
    {
        write(1,"\x1b[0m",4);
        if(i==start) {write(1, "\x1b[44mSTRT: ", 11); goto PRINTING;}
        else if(dist[i]<INT_MAX) {_INF goto PRINTING;}
        else goto NOTPRINTING;
        PRINTING:
            write(1,"node_id: ",9);
            char iii[10]; int leng=snprintf(iii,sizeof(iii),"%d",i); write(1, iii, leng); snprintf(iii, 10-leng, "          "); write(1,iii,10-leng); write(1,"previous_node: ",15);
            char prevus[10]; int lengg=snprintf(prevus,sizeof(prevus),"%d",prev[i]); write(1, prevus, lengg); snprintf(prevus, 10-lengg, "          "); write(1,prevus,10-lengg); write(1,"distance: ",10);
            char distace[10]; int lenggg=snprintf(distace,sizeof(distace),"%d",dist[i]); write(1, distace, lenggg); snprintf(distace, 10-lenggg, "          "); write(1,distace,10-lenggg);
            write(1,"\x1b[0m\n",5);
        NOTPRINTING:;
    }
    ///if(prev[finish]==-1) {_WRN printf("Shortest path between %d and %d could not be found, nodes are not connected.\n",start,finish);}
    PrintLongestPath(dist, prev, main_graph->vertice_number, start);
    position[start].cf.color = YELLOW;
    return 1;
}

void DrawBSTInOrder(struct BSDNode* root,int LeftOrRight,int previusX,int previusY,struct Author* data_list,Font myFont)
{
    int x = previusX;
    int y = previusY;
    char text[100];
    char text2[100];
    if (root != NULL)
    {

        if(LeftOrRight == 0){
            previusX -= 400;
            previusY += 200;
        }
        if(LeftOrRight == 1 ){
            previusX += 400;
            previusY += 200;
        }

        DrawBSTInOrder(root->left,0,previusX,previusY,data_list,myFont);
        if(LeftOrRight != -1)
        {
            DrawCircle(previusX,previusY,20,GREEN);
            DrawLine(previusX,previusY,x,y,PURPLE);
            sprintf(text,"%s    %d",data_list[root->value].name,root->value);
            DrawTextEx(myFont,text,(Vector2){previusX+20,previusY+20},20,1,WHITE);
        }


        DrawBSTInOrder(root->right,1,previusX,previusY,data_list,myFont);
        if(LeftOrRight != -1)
        {
            DrawCircle(previusX,previusY,20,GREEN);
            sprintf(text2,"%s    %d",data_list[root->value].name,root->value);
            DrawTextEx(myFont,text2,(Vector2){previusX+20,previusY+20},20,1,WHITE);
        }
    }
}
int GetInput(Font myFont){
    int SCREEN_WIDTH = 1000;
    int SCREEN_HEIGHT = 800;
    char register_parameter_1[50] = "\0";
    int letterCount = 0;
    while(!WindowShouldClose()){
        BeginDrawing();
        int key = GetCharPressed();

        if (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < 49)) {
                register_parameter_1[letterCount] = (char)key;
                register_parameter_1[letterCount + 1] = '\0';
                letterCount++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && (letterCount > 0)) {
            letterCount--;
            register_parameter_1[letterCount] = '\0';
        }
        if(IsKeyPressed(KEY_ENTER))
        {
            break;
        }

        DrawRectangle(SCREEN_WIDTH/2,SCREEN_HEIGHT/2 - 100,600,300,GRAY);
        DrawTextEx(myFont,"Enter author ID: ",(Vector2){SCREEN_WIDTH/2 + 50,SCREEN_HEIGHT/2 - 60},40
                   ,1,WHITE);
        DrawRectangle(SCREEN_WIDTH/2 + 50,+SCREEN_HEIGHT/2,500,100,BLACK);
        DrawTextEx(myFont,register_parameter_1,(Vector2){SCREEN_WIDTH/2 + 50,+SCREEN_HEIGHT/2},60,1,WHITE);
        EndDrawing();
        }
        while(!IsKeyReleased(KEY_ENTER)){BeginDrawing();EndDrawing();}
        return atoi(register_parameter_1);
}
