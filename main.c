#include "Parse.h"
#include "Graph.h"
#include "Registers.h"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"


#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 1000

int JustGiveRandomOnce = 1;

void FindTheCoauthorNumber(struct Graph* main_graph, struct Author* data_list, struct Position* position,int number);
void RandomPosition(struct Position* position,int numberOfAuthors,struct Author* data_list);
void DrawCirclesButBetter(struct Position* position,int numberOfAuthors,struct Graph* graph,Font MyFont);
void DrawButtons(struct Button* buttons,Camera2D camera);



int main()
{




    const int screen_height = 1000;
    const int screen_width = 1600;

    struct Graph* test_graph = CreateGraph(DATABASE_SIZE, 0);
    struct Author* test_data_list = (struct Author*)calloc(DATABASE_SIZE, sizeof(struct Author));
    memset(test_data_list,0,sizeof(*test_data_list));
    ExcelKatledici(test_data_list, test_graph);
    int actual_used = CountUsedIndexes(test_graph);
    actual_used += 10;
    _DBG printf("\n%d\n",actual_used);
    FreeGraph(test_graph);
    free(test_data_list);

    struct Graph* main_graph = CreateGraph(actual_used, 0);
    struct Author* data_list = (struct Author*)calloc(DATABASE_SIZE, sizeof(struct Author));
    memset(data_list,0,sizeof(*data_list));
    ExcelKatledici(data_list, main_graph);
    AssignEdgesToEveryone(main_graph, data_list, actual_used);

    for(int i=0; i<DATABASE_SIZE ;i++)
    {
        _INF
        printf("%d ",i);
        write(1, data_list[i].orcid, strlen(data_list[i].orcid));
        write(1, " , ", 3);
        write(1, data_list[i].name, strlen(data_list[i].name));
        write(1, "\n", 1);
    }
    PrintGraph(main_graph);
    int choice=0;
    int register_parameter_1;
    int register_parameter_2;

    struct Button* buttons = (struct Button*)malloc(sizeof(struct Button) * 7);/// 7 tane buton tut
    struct Position* position = (struct Position*)malloc(sizeof(struct Position) * main_graph->vertice_number-6);

    //////////////////////////////////////////////////////////////////////////////////

    ///Screen

    FindTheCoauthorNumber(main_graph,data_list,position,main_graph->vertice_number-6);
    RandomPosition(position,main_graph->vertice_number-6,data_list);

    ///Get the author list
    for(int i = 0 ; i < main_graph->vertice_number-6 ; i++){
           position[i].author = data_list[i];
    }

    InitWindow(screen_width,screen_height,"Data Set");
    Font myFont = LoadFont("Comic.ttf");
    SetTargetFPS(60);

    Camera2D camera = {0};
    camera.target = (Vector2){0,0};
    camera.rotation = 0.0f;
    camera.offset = (Vector2){screen_width/2,screen_height/2};
    camera.zoom = 1.0f;

    bool DrawTextBox = false;

    int clickedCircleIndex = -1;
    int WhichCircle = -1;
    int Scroll = 0;

    while(!WindowShouldClose()){
        int textInfo1[7];
        int textInfo2[7];
        int textInfo3[7];


        Vector2 mousePosition = GetMousePosition();
        mousePosition.x = (mousePosition.x / camera.zoom) + (camera.target.x - (camera.offset.x / camera.zoom));
        mousePosition.y = (mousePosition.y / camera.zoom) + (camera.target.y - (camera.offset.y / camera.zoom));

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < main_graph->vertice_number-6; i++) {
                    Vector2 CirclePosition = (Vector2){position[i].pos_x,position[i].pos_y};

                if (CheckCollisionPointCircle(mousePosition, CirclePosition, position[i].cf.radius)){
                    clickedCircleIndex = i;
                    WhichCircle = i;
                    break;
                }
            }
        }
        else
            clickedCircleIndex = -1;

        if(clickedCircleIndex != -1){
                position[clickedCircleIndex].pos_x = mousePosition.x;
                position[clickedCircleIndex].pos_y = mousePosition.y;
        }

        DrawButtons(buttons,camera);
        ///Fix the button position
        for(int i = 0 ; i < 7 ; i++){
            textInfo1[i] = buttons[i].pos_x + 40/camera.zoom;
            textInfo2[i] = buttons[i].pos_y + 47/camera.zoom;
            textInfo3[i] = 30/camera.zoom;
        }

        int TextWidht = 400/camera.zoom;
        int TextHeight = 1200/camera.zoom;
        int TextPos_x = (camera.target.x - (800/camera.zoom)) ;
        int TextPos_y = (camera.target.y - (600/camera.zoom));

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            for(int i = 0 ; i < 7 ; i++){
                Rectangle buttonPos = {buttons[i].pos_x,buttons[i].pos_y,buttons[i].widgt,buttons[i].height};
                if(CheckCollisionPointRec(mousePosition, buttonPos)){
                    switch(i+1)
                    {
                    case 1:
                        {


                            Register1(main_graph,data_list, GetInput(myFont), GetInput(myFont), 0,position);
                        break;
                        }

                    case 2:
                        {

                            Register2(main_graph, data_list, GetInput(myFont),position);

                        break;
                        }

                    case 3:
                        {
                            Register3(main_graph, data_list);
                        break;
                        }

                    case 4:
                        {
                            Register4(main_graph, data_list, GetInput(myFont),position);
                        break;
                        }

                    case 5:
                        {
                            int coop_no = 0;
                            coop_no = Register5(main_graph, data_list, GetInput(myFont));
                            char text[100];
                            sprintf(text,"Author has done a total of %d cooperations.\n",coop_no);
                            while(!WindowShouldClose()){
                                Font myFont = LoadFont("Comic.ttf");
                                BeginDrawing();
                                if(IsKeyPressed(KEY_ENTER))
                                {
                                    break;
                                }
                                DrawRectangle(1000/2,800/2 - 100,750,300,GRAY);
                                DrawTextEx(myFont,text,(Vector2){1000/2 + 50,+800/2,500},40,1,WHITE);
                                EndDrawing();
                                }
                                while(!IsKeyReleased(KEY_ENTER)){BeginDrawing();EndDrawing();}
                        break;
                        }

                    case 6:
                        {
                            int max_coops_id=0;
                            max_coops_id = Register6(main_graph, data_list);
                            _INF printf("The author with the most cooperations is \""); PrintNodeInfo(max_coops_id, data_list, 0); printf("\".\n");
                        break;
                        }
                    case 7:
                        {
                            Register7(main_graph, data_list, GetInput(myFont),position);
                        break;
                        }

                    }
                    goto EXIT;
                }
                else{
                    for(int i = 0 ; i < main_graph->vertice_number-6 ; i++){
                        if(position[i].author.orcid[0] != '\0')
                            position[i].cf.color = RED;
                        else
                            position[i].cf.color = BLUE;

                        }
                }
            }
        }
        EXIT:

        ///DRAW
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        if(!CheckCollisionPointRec(mousePosition,(Rectangle){TextPos_x,TextPos_y,TextWidht,TextHeight})){
            camera.zoom += ((float)GetMouseWheelMove()*0.05f);
        }

        if(CheckCollisionPointRec(mousePosition,(Rectangle){TextPos_x,TextPos_y,TextWidht,TextHeight})){
            Scroll += ((float)GetMouseWheelMove()*20.0f);
        }
        if(Scroll > 0 ) Scroll = 0;


        if(camera.zoom > 3.0f ) camera.zoom = 3.0f;
        else if (camera.zoom <= 0.01f) camera.zoom = 0.01f;

        DrawCirclesButBetter(position,main_graph->vertice_number-6,main_graph,myFont);
        ///Draw Buttons
        for(int i = 0 ; i < 7 ; i++){
            char button[8];
            sprintf(button,"Buton %d",i + 1);
            DrawRectangle(buttons[i].pos_x , buttons[i].pos_y , buttons[i].widgt, buttons[i].height,buttons[i].color);

            ///DrawText(button,textInfo1[i],textInfo2[i],textInfo3[i],WHITE);

            DrawTextEx(myFont,button,(Vector2){textInfo1[i],textInfo2[i]},textInfo3[i],2,WHITE);
        }
        ///If clicked position have a circle draw text box
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            for (int i = 0; i < main_graph->vertice_number-6; i++) {
                Vector2 CirclePosition = (Vector2){position[i].pos_x,position[i].pos_y};
                if (CheckCollisionPointCircle(mousePosition, CirclePosition, position[i].cf.radius)){
                    DrawTextBox = true;
                    break;
                }
                else
                    DrawTextBox = false;
            }
        }
        ///Open The Text Box

        if(DrawTextBox && position[WhichCircle].author.orcid[0] != '\0'){

            DrawRectangle(TextPos_x,TextPos_y,TextWidht,TextHeight,GRAY);
            int i = 0;

            int x_pos = TextPos_x + 30 /camera.zoom;
            int y_pos = TextPos_y + 120/camera.zoom;
            int TextSize = 20/camera.zoom;
            int spaceBetweenCharacters = 5/camera.zoom;
            int y_space = 0 /camera.zoom;

            int paperNumber = 0;

            for(int i = 0 ; i < PAPER_LIMIT ; i++){if(position[WhichCircle].author.papers[i].name[0] != 0) paperNumber++;}
            ///HER MAKALEYI GEZ
            Vector2 stats = (Vector2){TextPos_x + spaceBetweenCharacters + 40/camera.zoom - 20/camera.zoom,TextPos_y + 110/camera.zoom + y_space};
            for(int i = 0 ; i < paperNumber ; i++){
                ///Yazar ismi ve idsini yazdir
                if(i == 0){

                    DrawTextEx(myFont,position[WhichCircle].author.name,stats,30/camera.zoom,1,WHITE);
                    stats.y += 40/camera.zoom;
                    DrawTextEx(myFont,position[WhichCircle].author.orcid,stats,30/camera.zoom,1,WHITE);
                    stats.y += 40/camera.zoom;

                    char ID[16];
                    sprintf(ID, "%d", WhichCircle);
                    DrawTextEx(myFont,ID,stats,30/camera.zoom,1,WHITE);
                }

                ///HARF HARF TARA
                int j = 0;
                int Text_Xpos = TextPos_x + spaceBetweenCharacters + 40/camera.zoom - 20/camera.zoom;
                int Text_Ypos = TextPos_y + 232/camera.zoom + y_space;
                int Text_zoom = 30/camera.zoom;

                ///DrawText(".",Text_Xpos,Text_Ypos,Text_zoom,WHITE);
                if( TextPos_y + 240/camera.zoom + y_space + Scroll - 35/camera.zoom> stats.y )
                    DrawTextEx(myFont,".",(Vector2){Text_Xpos,Text_Ypos + Scroll},Text_zoom,2,WHITE);
                ///Karakterleri teker teker yazdir.
                while(position[WhichCircle].author.papers[i].name[j] != '\0'){
                    char myChar[2];

                    myChar[0] = position[WhichCircle].author.papers[i].name[j];
                    myChar[1] = '\0';

                    if(spaceBetweenCharacters*camera.zoom < 350){
                        ///DrawText(myChar,TextPos_x + spaceBetweenCharacters + 40/camera.zoom,TextPos_y + 150/camera.zoom + y_space,10/camera.zoom,WHITE);
                        Vector2 MyVector = (Vector2){TextPos_x + spaceBetweenCharacters + 40/camera.zoom,TextPos_y + 240/camera.zoom + y_space + Scroll};
                        if( TextPos_y + 240/camera.zoom + y_space + Scroll - 35/camera.zoom > stats.y ){
                            DrawTextEx(myFont,myChar,MyVector,22/camera.zoom,2,WHITE);
                        }

                    }
                    else{
                        spaceBetweenCharacters = 5/camera.zoom;
                        y_space += 30/camera.zoom;
                    }
                    spaceBetweenCharacters += 10/camera.zoom;
                    j++;
                }
                spaceBetweenCharacters = 5/camera.zoom;
                y_pos = y_space += 30/camera.zoom;

            }
            i++;
        }
        EndMode2D();
        EndDrawing();
    }
    UnloadFont(myFont);
    CloseWindow();
}
void RandomPosition(struct Position* position,int numberOfAuthors,struct Author* data_list){
    ///Coauthorlara göre özellik atamasi yap.
    for(int i  = 0 ; i < numberOfAuthors ; i ++){

        if(position[i].cf.NumberOfCoauthors == 0){
            position[i].cf.color = WHITE;
            position[i].cf.radius = 20;
        }
        else if(position[i].cf.NumberOfCoauthors < 40){
            position[i].cf.color = BLUE;
            position[i].cf.radius = 50;
        }
        else if(position[i].cf.NumberOfCoauthors >= 40){
            position[i].cf.color = RED;
            position[i].cf.radius = 100;
        }
    }


    for(int i = 0 ; i < numberOfAuthors ; i++){
        if(position[i].cf.radius == 100){
            ///4000
            ///3200
            position[i].pos_x = -(SCREEN_WIDTH*2) + rand()%(SCREEN_WIDTH*4);
            position[i].pos_y = (SCREEN_HEIGHT*2) - rand()%(SCREEN_HEIGHT*4);
            position[i].author = data_list[i];
        }
        else if(position[i].cf.radius == 50){

                ///1000 ve 800

                position[i].pos_x = -(SCREEN_WIDTH*8) + rand()%(SCREEN_WIDTH*16);
                position[i].pos_y = (SCREEN_HEIGHT*8) - rand()%(SCREEN_HEIGHT*16);
                position[i].author = data_list[i];

        }
        else if(position[i].cf.radius == 20){
            position[i].pos_x = 10000 + rand()%(SCREEN_WIDTH);
            position[i].pos_y = (SCREEN_HEIGHT) - rand()%(SCREEN_HEIGHT*2);
            position[i].author = data_list[i];
        }
    }


}


void FindTheCoauthorNumber(struct Graph* main_graph, struct Author* data_list, struct Position* position,int number)
{
    for(int i  = 0 ; i < number ; i ++){

        struct Node* current = main_graph->list_of_adjacency_lists[i];
        int coauthor_no = 0;

        while (current != NULL)  // Traverse the adjacency list
        {
            coauthor_no++;  // Count the number of coauthors
            current = current->next;  // Move to the next node
        }
        position[i].cf.NumberOfCoauthors = coauthor_no;
    }
}
void DrawCirclesButBetter(struct Position* position,int numberOfAuthors,struct Graph* graph,Font MyFont)
{

    int flag = 1;
    int xStartPosition = 0;
    int yStartPosition = 0;
    for(int i = 0 ;i < numberOfAuthors ; i++ )
        position[i].IsDrawed = false;

    for(int i = 0 ; i < numberOfAuthors ; i++){
        if(position[i].author.orcid[0] != '\0'){
            ///Get the node
            struct Node* current = graph->list_of_adjacency_lists[i];
            if(JustGiveRandomOnce){
                if(position[i].IsDrawed != true){
                    position[i].pos_x = xStartPosition;
                    position[i].pos_y = yStartPosition;
                    position[i].cf.radius = 100;
                    position[i].cf.color = RED;
                }
            }

        while (current != NULL)
        {

            if(position[current->node_id].IsDrawed != true){
                if(JustGiveRandomOnce == 1){
                    position[current->node_id].cf.color = BLUE;
                    position[current->node_id].cf.radius = 20;
                    position[current->node_id].pos_x = (position[i].pos_x - 2700) + rand()%(5400);
                    position[current->node_id].pos_y = (position[i].pos_y - 2700) + rand()%(5400);
                    position[current->node_id].IsDrawed = true;
                }
            }

            DrawLine(position[i].pos_x,position[i].pos_y,position[current->node_id].pos_x,position[current->node_id].pos_y,PURPLE);
            DrawCircle(position[current->node_id].pos_x,position[current->node_id].pos_y,position[current->node_id].cf.radius, position[current->node_id].cf.color);
            DrawText(position[current->node_id].author.name,position[current->node_id].pos_x + position[current->node_id].cf.radius,
            position[current->node_id].pos_y + position[current->node_id].cf.radius,20,YELLOW);

            current = current->next;
        }
        ///AUTHOR
        DrawCircle(position[i].pos_x,position[i].pos_y,position[i].cf.radius,position[i].cf.color);
        DrawTextEx(MyFont,position[i].author.name,(Vector2){position[i].pos_x + position[i].cf.radius,position[i].pos_y + position[i].cf.radius},
                   20,2,YELLOW);



        xStartPosition += 5000;
        flag++;
            if(flag == 11){
                xStartPosition = 0;
                yStartPosition += 5000;
                flag = 1;
            }
        }
    }
    JustGiveRandomOnce = 0;
}
void DrawButtons(struct Button* buttons,Camera2D camera){

    //DrawRectangle(camera.target.x + (SCREEN_WIDTH / 2.0f),(camera.target.y - (SCREEN_HEIGHT / 2.0f)),240,SCREEN_HEIGHT,WHITE);


    for(int i = 0 ; i < 7 ; i++){

        buttons[i].color = GRAY;
        buttons[i].height = 120;
        buttons[i].widgt = 200;


        buttons[i].pos_x =  (camera.target.x + (SCREEN_WIDTH / 2.0f - buttons[i].widgt + 80) / camera.zoom) + 200/camera.zoom;
        buttons[i].pos_y = (camera.target.y - (SCREEN_HEIGHT / 2.0f -  20) / camera.zoom) + (140*i)/camera.zoom - 100/camera.zoom;

        buttons[i].widgt = buttons[i].widgt / camera.zoom;
        buttons[i].height = buttons[i].height / camera.zoom;

    }
}
