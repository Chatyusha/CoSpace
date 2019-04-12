////////////////////////////////////////
//
//	File : ai.c
//	CoSpace Robot
//	Version 1.0.0
//	Jan 1 2016
//	Copyright (C) 2016 CoSpace Robot. All Rights Reserved
//
//////////////////////////////////////
//
// ONLY C Code can be compiled.
//
/////////////////////////////////////

#define CsBot_AI_H//DO NOT delete this line
#ifndef CSBOT_REAL
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DLL_EXPORT extern __declspec(dllexport)
#define false 0
#define true 1
#endif//The robot ID : It must be two char, such as '00','kl' or 'Cr'.
char AI_MyID[2] = {'0','1'};

int Duration = 0;
int SuperDuration = 0;
int bGameEnd = false;
int CurAction = -1;
int CurGame = 0;
int SuperObj_Num = 0;
int SuperObj_X = 0;
int SuperObj_Y = 0;
int Teleport = 0;
int LoadedObjects = 0;
int US_Front = 0;
int US_Left = 0;
int US_Right = 0;
int CSLeft_R = 0;
int CSLeft_G = 0;
int CSLeft_B = 0;
int CSRight_R = 0;
int CSRight_G = 0;
int CSRight_B = 0;
int PositionX = 0;
int PositionY = 0;
int TM_State = 0;
int Compass = 0;
int Time = 0;
int WheelLeft = 0;
int WheelRight = 0;
int LED_1 = 0;
int MyState = 0;
int AI_SensorNum = 13;

#define CsBot_AI_C//DO NOT delete this line

//#DEFINES of COLORS
#define COLOR_YELLOW 0;
#define COLOR_RED 1;
#define COLOR_CYAN 2;
#define COLOR_BLACK 3;
#define COLOR_SUPER_OBJ 4;
#define COLOR_DEPOSIT 5;
#define COLOR_BLUE_FLOOR 6;
#define COLOR_SWAMPLAND 7;
#define COLOR_IN_TRAP 8;
#define COLOR_TYPE 9;
//#DEFINES of Dijkstra
#define MAX_DOT_NUMBER 36*27;

enum Action_W1//CurActions for WORLD_1
{
  DEFINE = 0,
  FIND_OBJ,
  GO
};
enum Action_W1 action_w1;

enum Action_W2//CurActions for WORLD_2
{
  DEFINE = 0,
  GO
};

enum Action_W2 action_w2;
// Global_Variable
 //Colors
int red_obj[3][2] = {{230, 260}, {25, 40}, {25, 40}};
int cyan_obj[3][2] = {{25, 40}, {245, 255}, {250, 255}};
int black_obj[3][2] = {{25, 40}, {25, 40}, {25, 40}};
int trap_line[3][2] = {{200, 240}, {217, 248}, {0, 5}};
int blue_zone[3][2] = {{0, 0}, {150, 171}, {255, 255}};
int object_box[3][2] = {{230, 240}, {140, 150}, {0, 5}};
int gray_zone[3][2] = {{130, 155}, {140, 165}, {185, 210}};
int sp_obj[3][2] = {{230, 255}, {15, 45}, {250, 255}};
int purple_line[3][2] = {{150, 180}, {80, 100}, {180, 220}};
int in_trap[3][2] = {{50,70},{95,115},{220,250}};
int Yellow[3][2] = {{195, 245}, {215, 250}, {0, 5}};
int trap[3][2] = {{50,70},{95,120},{22,250}};

int UsingColors[COLOR_TYPE][6];
 // others
int loaded_objects[4];//[0]==red [1]==cyan [2]==break [3]==super_object
// GLOBAL_FUNCTION
 //  Declaration Funtions//
int ColorInformationInputer(void);
int IsOnStuff(void);
int IsOnRedObj(void);
int IsOnCyanObj(void);
int IsOnBlackObj(void);
void Motor(int left,int right);

void Motor(int left,int right)
{
  action_w1 = DEFINE;
  action_w2 = DEFINE;
  WheelLeft = left;
  WheelRight = right;
  LED_1 = 0;
  MyState = 0;
}
 // other functions

int ColorInformationInputer(int num , int col[3][2]){
  for (int i=0; i < 3; i++) {
    for (int j=0;j<2;j++){
      UsingColors[num][3*i+j]=col[i][j];
    }
  }
}
int IsOnStuff(int num){
  int r = 0;
  int color_width = 10;
  if(num==COLOR_SUPER_OBJ){
    color_width = 20;
  }
  if(Colors[num][0]-color_width <= CSLeft_R && CSLeft_R <= Colors[num][1]+color_width
  && Colors[num][2]-color_width <= CSLeft_G && CSLeft_G <= Colors[num][3]+color_width
  && Colors[num][4]-color_width <= CSLeft_B && CSLeft_B <= Colors[num][5]+color_width){
    r++;
  }
  if(Colors[num][0]-color_width <= CSRight_R && CSRight_R <= Colors[num][1]+color_width
  && Colors[num][2]-color_width <= CSRight_G && CSRight_G <= Colors[num][3]+color_width
  && Colors[num][4]-color_width <= CSRight_B && CSRight_B <= Colors[num][5]+color_width){
    r+=2;
  }
  return r;
}
int IsOnRedObj(){
  return IsOnStuff(COLOR_RED);
}
int IsOnCyanObj(){
  return IsOnStuff(COLOR_CYAN);
}
int IsOnBlackObj(){
  return IsOnStuff(COLOR_BLACK);
}
//functions for world_1

//WORLD_2
 //Variables
struct Dot{
  int id;//dot_id
  int cost;//そのdotに行くまでのcost
  int from;//直前のdot
  int done;//そのdotを調べたかどうか
  int edge_to[MAX_DOT_NUMBER];//そのdotから行くことの出来るdotのid
  int edge_cost[MAX_DOT_NUMBER];//edge_toに行くための道のコスト
  int edge_num;//そのdotから行くことの出来るdotの数
}
DLL_EXPORT void SetGameID(int GameID)
{
    CurGame = GameID;
    bGameEnd = 0;
}

DLL_EXPORT int GetGameID()
{
    return CurGame;
}

//Only Used by CsBot Dance Platform
DLL_EXPORT int IsGameEnd()
{
    return bGameEnd;
}

#ifndef CSBOT_REAL

DLL_EXPORT char* GetDebugInfo()
{
    char info[1024];
    sprintf(info, "Duration=%d;SuperDuration=%d;bGameEnd=%d;CurAction=%d;CurGame=%d;SuperObj_Num=%d;SuperObj_X=%d;SuperObj_Y=%d;Teleport=%d;LoadedObjects=%d;US_Front=%d;US_Left=%d;US_Right=%d;CSLeft_R=%d;CSLeft_G=%d;CSLeft_B=%d;CSRight_R=%d;CSRight_G=%d;CSRight_B=%d;PositionX=%d;PositionY=%d;TM_State=%d;Compass=%d;Time=%d;WheelLeft=%d;WheelRight=%d;LED_1=%d;MyState=%d;",Duration,SuperDuration,bGameEnd,CurAction,CurGame,SuperObj_Num,SuperObj_X,SuperObj_Y,Teleport,LoadedObjects,US_Front,US_Left,US_Right,CSLeft_R,CSLeft_G,CSLeft_B,CSRight_R,CSRight_G,CSRight_B,PositionX,PositionY,TM_State,Compass,Time,WheelLeft,WheelRight,LED_1,MyState);
    return info;
}

DLL_EXPORT char* GetTeamName()
{
     return "WorkingCell";
}

DLL_EXPORT int GetCurAction()
{
    return CurAction;
}

//Only Used by CsBot Rescue Platform
DLL_EXPORT int GetTeleport()
{
    return Teleport;
}

//Only Used by CsBot Rescue Platform
DLL_EXPORT void SetSuperObj(int X, int Y, int num)
{
    SuperObj_X = X;
    SuperObj_Y = Y;
    SuperObj_Num = num;
}
//Only Used by CsBot Rescue Platform
DLL_EXPORT void GetSuperObj(int *X, int *Y, int *num)
{
    *X = SuperObj_X;
    *Y = SuperObj_Y;
    *num = SuperObj_Num;
}

#endif ////CSBOT_REAL

DLL_EXPORT void SetDataAI(volatile int* packet, volatile int *AI_IN)
{

    int sum = 0;

    US_Front = AI_IN[0]; packet[0] = US_Front; sum += US_Front;
    US_Left = AI_IN[1]; packet[1] = US_Left; sum += US_Left;
    US_Right = AI_IN[2]; packet[2] = US_Right; sum += US_Right;
    CSLeft_R = AI_IN[3]; packet[3] = CSLeft_R; sum += CSLeft_R;
    CSLeft_G = AI_IN[4]; packet[4] = CSLeft_G; sum += CSLeft_G;
    CSLeft_B = AI_IN[5]; packet[5] = CSLeft_B; sum += CSLeft_B;
    CSRight_R = AI_IN[6]; packet[6] = CSRight_R; sum += CSRight_R;
    CSRight_G = AI_IN[7]; packet[7] = CSRight_G; sum += CSRight_G;
    CSRight_B = AI_IN[8]; packet[8] = CSRight_B; sum += CSRight_B;
    PositionX = AI_IN[9]; packet[9] = PositionX; sum += PositionX;
    PositionY = AI_IN[10]; packet[10] = PositionY; sum += PositionY;
    TM_State = AI_IN[11]; packet[11] = TM_State; sum += TM_State;
    Compass = AI_IN[12]; packet[12] = Compass; sum += Compass;
    Time = AI_IN[13]; packet[13] = Time; sum += Time;
    packet[14] = sum;

}
DLL_EXPORT void GetCommand(int *AI_OUT)
{
    AI_OUT[0] = WheelLeft;
    AI_OUT[1] = WheelRight;
    AI_OUT[2] = LED_1;
    AI_OUT[3] = MyState;
}
void Game0()
{

    if(SuperDuration>0)
    {
        SuperDuration--;
    }
    else if(Duration>0)
    {
        Duration--;
    }
    else if(IsOnRedObj()&&LoadedObjects<6&&loaded_objects[0]<2){
      loaded_objects[0]++;
      printf("\nFind Red Object");
      Duration = 49;
      action_w1 =FIND_OBJ;
    }
    else if(IsOnCyanObj()&&LoadedObjects<6&&loaded_objects[1]<2){
      loaded_objects[1]++;
      printf("\nFind Cyan Object");
      Duration = 49;
      action_w1 =FIND_OBJ;
    }
    else if(IsOnBlackObj()&&LoadedObjects<6&&loaded_objects[2]<2){
      loaded_objects[2]++;
      printf("\nFind Black Object");
      Duration = 49;
      action_w1 = FIND_OBJ;
    }
    else if(true)
    {
        Duration = 0;
        action_w1 = GO;
    }
    switch(action_w1)
    {
        case FIND_OBJ:
            WheelLeft = 0;
            WheelRight = 0;
            LED_1 = 1;
            MyState = 0;
            if(Duration==1){
              LoadedObjects++;
            }
            if(Duration<6){
              WheelLeft = 1;
              WheelRight = 1;
              LED_1 = 0;
              MyState = 0;
            }
            break;
        case GO:
            if(US_Left>25){
              Motor(2,3);
            }else if(US_Left<15){
              Motor(3,2);
            }else{
              Motor(3,3);
            }
            if(US_Front<20){
              Motor(3,0);
            }
            break;
        default:
            break;
    }

}

void Game1()
{

    if(SuperDuration>0)
    {
        SuperDuration--;
    }
    else if(Duration>0)
    {
        Duration--;
    }
    else if(true)
    {
        Duration = 0;
        action_w2 = GO;
    }
    switch(action_w2)
    {
        case GO:
            Motor(3,3);
            break;
        default:
            break;
    }

}


DLL_EXPORT void OnTimer()
{
    switch (CurGame)
    {
        case 9:
            break;
        case 10:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            MyState=0;
            break;
        case 0:
            Game0();
            break;
        case 1:
            Game1();
            break;
        default:
            break;
    }
}
