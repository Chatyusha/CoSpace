//////////////////////////////////////////
//	File : amain_.c
//	CoSpace Robot
//	Version 1.0.0
//	Jan 1 2016
//	Copyright (C) 2016 CoSpace Robot. All Rights Reserved
//
//
//////////////////////////////////////
//
// ONLY C Code can be compiled.
//
/////////////////////////////////////
// Manual
// GetDebugInfo() and GetTeamName() was changed
/////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#define CsBot_AI_H //DO NOT delete this line
#ifndef CSBOT_REAL
#include <windows.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define DLL_EXPORT extern __declspec(dllexport)
#define false 0
#define true 1
#endif //The robot ID : It must be two char, such as '00','kl' or 'Cr'.
char AI_MyID[2] = {'0', '2'};

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
int tereport_wait = 0;
int loaded_objects[4];
int instrumentation = 1;
int identification = 0;
int near_DEPOSIT = 0;
int want_object = 2;
int log_posX = 0;
int log_posY = 0;
int testcmd = 0;
int back_flag = 0;
int curcolor = 0;

int red_obj[3][2] = {{230, 260}, {25, 40}, {25, 40}};
int cyan_obj[3][2] = {{25, 40}, {245, 255}, {250, 255}};
int black_obj[3][2] = {{25, 40}, {25, 40}, {25, 40}};
int trap_line[3][2] = {{200, 240}, {217, 248}, {0, 5}};
int blue_zone[3][2] = {{0, 0}, {150, 171}, {255, 255}};
int object_box[3][2] = {{204, 235}, {130, 150}, {0, 5}};
int gray_zone[3][2] = {{130, 155}, {140, 165}, {185, 210}};
int sp_obj[3][2] = {{230, 255}, {15, 45}, {250, 255}};
int purple_line[3][2] = {{150, 180}, {80, 100}, {180, 220}};
int in_trap[3][2] = {{50,70},{95,115},{220,250}};
int Yellow[3][2] = {{195, 245}, {215, 250}, {0, 5}};
int trap[3][2] = {{50,70},{95,120},{22,250}};

int Renge_Random(int min,int max){
  return rand()%(max-min)+min;
}

enum Action {
    DEFIND = 0,
    FIND_OBJ,
    TELEPORT,
    DEPOSIT_OBJ,
    YELLOW_AVOIDANCE,
    AVOID,
    GO
};
enum Action action;

void InputColorInformation(void);
int SetColor(int col[3][2],int status);
int IsOnYellowLine(void);
int IsOnRedObj(void);
int IsOnCyanObj(void);
int IsOnBlackObj(void);
int IsOnSuperObj(void);
int IsOnBlueFloor(void);
int IsOnDepositArea(void);
int IsOnSwampland(void);
int IsOnGreenMaker(void);
int Obstacle(int left, int front, int right);
void Motor(int left, int right);

#define POINT_BLACK 20
#define POINT_RED 10
#define POINT_CYAN 15
#define POINT_SUPERAREA 2
#define POINT_WHITE 1
#define POINT_DEPOSIT 0
#define POINT_SWAMPLAND -1
#define POINT_WALL -2
#define POINT_YELLOW -3

#define COSPACE_WIDTH 360
#define COSPACE_HEIGHT 270
#define SIZE 10
#define DOT_WIDTH_NUMBER (COSPACE_WIDTH / SIZE)
#define DOT_HEIGHT_NUMBER (COSPACE_HEIGHT / SIZE)
#define MAX_DOT_NUMBER (COSPACE_WIDTH * COSPACE_HEIGHT / SIZE / SIZE)
#define MAX_EDGE_NUMBER 25
#define BORDER_SAME_OBJECT_NUMBER 2
#define FIND_OBJECT_DURATION 50
#define PLUSMINUS(a, b, c) ((a) <= (b) + (c) && (a) >= (b) - (c))
#define PI 3.14

FILE *logfile;
FILE *errfile;
FILE *Motor_file;

int repeated_num = 0;
int repeated_num2 = 0;
int repeated_num3 = 0;
int repeated_num4 = 0;
int log_x, log_y, log_compass;
int log_superobj_num, log_superobj_x[10], log_superobj_y[10];
int absolute_x = -1, absolute_y = -1;
int now_dot_id;
int emergency_now_dot_id = 166;
// int process = -1;
int super_sameoperate = 0;
//[WheelLeft][WheelRight][0]:distance
//[WheelLeft][WheelRight][1]:angle
int Motor_log[11][11][2];

struct Dot
{
    int x, y;                       //dotのx(0<=x<36), y(0<=y<27)座標
    int wide;                       //一辺の長さ
    int point;                      //ドットの種類(-3:yellow -2:wall etc.)
    int done;                       //Dijkstra()
    int id;                         //y * 36 + x
    int from;                       //Dijkstra()
    int cost;                       //Dijkstra()
    int distance_from_start;        //Dijkstra()
    int curved_times;               //Dijkstra()
    int arrived_times;              //そこにいた回数
    int edge_num;                   //そのドットに行くことのできるドットの数
    int edge_to[MAX_EDGE_NUMBER];   //
    int edge_cost[MAX_EDGE_NUMBER]; //
    int red;                        //もし、Redがとれるなら、1
    int cyan;                       //もし、Cyanがとれないなら0
    int black;                      //もし、Blackが...
    int color;
};
struct Dot dot[MAX_DOT_NUMBER];

void AvoidWall();
void AvoidYellow_Line();
void AvoidEdge();
void SetTown();
void AvoidYellow_Line_W1();
void AvoidWall_W1();
void AvoidEdge2();
void Finding_obj();
int IsNearYellow(int num, int x, int y);

int same_target_num = 5;
int same_target = 0;

int SetColor(int col[3][2],int status){
  if(status == 0){
    // either
  return col[0][0] <= CSLeft_R && CSLeft_R <= col[0][1]
      && col[1][0] <= CSLeft_G && CSLeft_G <= col[1][1]
      && col[2][0] <= CSLeft_B && CSLeft_B <= col[2][1]
      || col[0][0] <= CSRight_R && CSRight_R <= col[0][1]
      && col[1][0] <= CSRight_G && CSRight_G <= col[1][1]
      && col[2][0] <= CSRight_B && CSRight_B <= col[2][1] ;
    }
    else if(status==1){
      return col[0][0] <= CSLeft_R && CSLeft_R <= col[0][1]
          && col[1][0] <= CSLeft_G && CSLeft_G <= col[1][1]
          && col[2][0] <= CSLeft_B && CSLeft_B <= col[2][1]
          && col[0][0] <= CSRight_R && CSRight_R <= col[0][1]
          && col[1][0] <= CSRight_G && CSRight_G <= col[1][1]
          && col[2][0] <= CSRight_B && CSRight_B <= col[2][1] ;
    }

}
#define REDROBOT 0;
#define BLUEROBOT 1;
int started_start_movement = true;
int Im_in_sentor = false;
int onemore = true;
int onemore2 = true;

void Game0()
{
    InputColorInformation();
    if(SetColor(trap,0)){
      LoadedObjects=0;
      loaded_objects[0]=0;
      loaded_objects[1]=0;
      loaded_objects[2]=0;
    }
    if (SuperDuration > 0)
    {
        SuperDuration--;
    }
    else if(Duration > 0){
      Duration--;
    }
    else if(SetColor(Yellow,0)&&LoadedObjects!=0){
      Duration = 5;
      action = YELLOW_AVOIDANCE;
    }
    else if(SetColor(object_box,1)){
      if(Time<90){
        if(LoadedObjects==6){
          Duration = 54;
          action = DEPOSIT_OBJ;
        }
      }else{
        Duration = 54;
        action = DEPOSIT_OBJ;
      }
    }
    else if(SetColor(red_obj,0)){
      if(Time<90){
        if(loaded_objects[0]<2){
          Duration = 49;
          loaded_objects[0]++;
          LoadedObjects++;
          action = FIND_OBJ;
        }
      }else{
        Duration = 49;
        loaded_objects[0]++;
        LoadedObjects++;
        action = FIND_OBJ;
      }
    }
    else if(SetColor(cyan_obj,0)){
      if(Time<90){
        if(loaded_objects[1]<2){
          Duration = 49;
          loaded_objects[1]++;
          LoadedObjects++;
          action = FIND_OBJ;
        }
      }else{
        Duration = 49;
        loaded_objects[1]++;
        LoadedObjects++;
        action = FIND_OBJ;
      }
    }
    else if(SetColor(black_obj,0)){
      if(Time<90){
        if(loaded_objects[2]<2){
          Duration = 49;
          loaded_objects[2]++;
          LoadedObjects++;
          action = FIND_OBJ;
        }
      }else{
        Duration = 49;
        loaded_objects[2]++;
        LoadedObjects++;
        action = FIND_OBJ;
      }
    }else if(US_Front<40){
      Duration = 0 ;
      action = AVOID;
    }
    else if(true){
      Duration = 0;
      action = GO;
    }

    switch (action)
    {
      case FIND_OBJ:
           WheelLeft = 0;
           WheelRight = 0;
           LED_1 = 1;
           MyState = 0;
           if(Duration<6){
             WheelLeft = 2;
             WheelRight = 2;
             LED_1 = 0;
             MyState = 0;
           }
           break;
      case DEPOSIT_OBJ:
           WheelLeft = 0;
           WheelRight = 0;
           LED_1 = 2;
           MyState = 0;
           if(Duration<6){
             WheelLeft = -3;
             WheelRight = -3;
             LED_1 = 0;
             MyState = 0;
           }
           if(Duration==1){
             LoadedObjects=0;
             loaded_objects[0]=0;
             loaded_objects[1]=0;
             loaded_objects[2]=0;
           }
           break;
      case YELLOW_AVOIDANCE:
           WheelLeft = -3;
           WheelRight = -3;
           LED_1 = 0;
           MyState = 0;
           if(Duration<4){
             if(LoadedObjects > 5||Time>90){
               WheelLeft = 2;
               WheelRight = -2;
               LED_1=0;
               MyState=0;
             }
             else
             {
             WheelLeft=-2;
             WheelRight=2;
             LED_1=0;
             MyState=0;
             }
           }
           break;
      case AVOID:
            Motor(3,0);
            break;
      case GO:
           if(US_Left<40){
             Motor(4,2);
           }else if(US_Left>60){
             Motor(2,4);
           }else{
             Motor(2,4);
           }
           break;
      default:
           break;
    }
}

int large_process = -1;

FILE *object_file;

int R_yellow(){
  return CSRight_R >= Yellow[0][0] && CSRight_R <= Yellow[0][1] && CSRight_G >= Yellow[1][0] && CSRight_G <= Yellow[1][1] && CSRight_B >= Yellow[2][0] && CSRight_B <= Yellow[2][1];
}
int L_yellow(){
  return CSLeft_R >= Yellow[0][0] && CSLeft_R <= Yellow[0][1] && CSLeft_G >= Yellow[1][0] && CSLeft_G <= Yellow[1][1] && CSLeft_B >= Yellow[2][0] && CSLeft_B <= Yellow[2][1];
}
int trap_zone (){
    return  CSLeft_R >= trap[0][0] && CSLeft_R <= trap[0][1] && CSLeft_G >= trap[1][0] && CSLeft_G <= trap[1][1] && CSLeft_B >= trap[2][0] && CSLeft_B <= trap[2][1] ||
             CSRight_R >= trap[0][0] && CSRight_R <= trap[0][1] && CSRight_G >= trap[1][0] && CSRight_G <= trap[1][1] && CSRight_B >= trap[2][0] && CSRight_B <= trap[2][1];
}
int Obj_Box(){
    return CSLeft_R >= object_box[0][0] && CSLeft_R <= object_box[0][1] && CSLeft_G >= object_box[1][0] && CSLeft_G <= object_box[1][1] && CSLeft_B >= object_box[2][0] && CSLeft_B <= object_box[2][1] &&
             CSRight_R >= object_box[0][0] && CSRight_R <= object_box[0][1] && CSRight_G >= object_box[1][0] && CSRight_G <= object_box[1][1] && CSRight_B >= object_box[2][0] && CSRight_B <= object_box[2][1];
}
int SP_obj(){
  return CSLeft_R >= sp_obj[0][0] && CSLeft_R <= sp_obj[0][1] && CSLeft_G >= sp_obj[1][0] && CSLeft_G <=sp_obj[1][1] && CSLeft_B >= sp_obj[2][0] && CSLeft_B <= sp_obj[2][1] ||
         CSRight_R >= sp_obj[0][0] && CSRight_R <= sp_obj[0][1] && CSRight_G >= sp_obj[1][0] && CSRight_G <=sp_obj[1][1] && CSRight_B >= sp_obj[2][0] && CSRight_B <= sp_obj[2][1];
}

int WallSensor(){
  return US_Left<10||US_Front<10||US_Right<10;
}
int Super_WallSensor(int left,int front,int right){
  return US_Left<left||US_Front<front||US_Right<right;
}

int wall(int left, int front, int right){
    int r = 0;
    if (US_Left < left)
    {
        r++;
    }
    if (US_Front < front)
    {
        r += 2;
    }
    if (US_Right < right)
    {
        r += 4;
    }
    return r;
}

int edge(){
  return (PositionX!=0&&PositionY!=0&&PositionX<20)||PositionX>340||(PositionX!=0&&PositionY!=0&&PositionY<20)||PositionY>250;
}


int SetAngle(int x,int y){
  double angle = atan2(y-log_posY, x-log_posX);
  // ラジアン -> 普通の角度
  angle = angle * 180 / 3.14;
  int int_angle = angle;
  if(int_angle<0){
    int_angle+=360;//int_angle...0~360
  }
  int_angle+=270;
  int_angle=int_angle%360;//Compassと同じ
  return int_angle;
}

int Arrived_Position(int x,int y,int judge_x,int judge_y){
  return x-judge_x<PositionX&&PositionX<x+judge_x&&y-judge_y<PositionY&&PositionY<y+judge_y;
}

#define COLOR_TYPE_NUMBER 9
#define COLOR_YELLOW 0
#define COLOR_RED 1
#define COLOR_CYAN 2
#define COLOR_BLACK 3
#define COLOR_SUPER_OBJ 4
#define COLOR_BLUE_FLOOR 5
#define COLOR_DEPOSIT 6
#define COLOR_SWAMPLAND 7
#define COLOR_IN_TRAP 8
int color_world2[COLOR_TYPE_NUMBER][6];
int ColorInformationInputer(int num, int col[3][2])
{
    if (num >= COLOR_TYPE_NUMBER)
    {
        return ERROR;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (col[i][j] < 0 || col[i][j] > 255)
            {
            }
            color_world2[num][j * 3 + i] = col[i][j];
        }
    }
    return 1;
}
void InputColorInformation(void)
{
    ColorInformationInputer(COLOR_YELLOW, trap_line);
    ColorInformationInputer(COLOR_RED, red_obj);
    ColorInformationInputer(COLOR_CYAN, cyan_obj);
    ColorInformationInputer(COLOR_BLACK, black_obj);
    ColorInformationInputer(COLOR_SUPER_OBJ, sp_obj);
    ColorInformationInputer(COLOR_BLUE_FLOOR, blue_zone);
    ColorInformationInputer(COLOR_DEPOSIT, object_box);
    ColorInformationInputer(COLOR_SWAMPLAND, gray_zone);
    ColorInformationInputer(COLOR_IN_TRAP, in_trap);
}

int IsOnStuff(int num)
{
    int ans = 0;
    int color_width = 10;
    if (num == COLOR_SUPER_OBJ)
    {
        color_width += 20;
    }
    if ((color_world2[num][0] + color_width > CSLeft_R && color_world2[num][0] - color_width < CSLeft_R && color_world2[num][1] + color_width > CSLeft_G && color_world2[num][1] - color_width < CSLeft_G && color_world2[num][2] + color_width > CSLeft_B && color_world2[num][2] - color_width < CSLeft_B) || (color_world2[num][3] + color_width > CSLeft_R && color_world2[num][3] - color_width < CSLeft_R && color_world2[num][4] + color_width > CSLeft_G && color_world2[num][4] - color_width < CSLeft_G && color_world2[num][5] + color_width > CSLeft_B && color_world2[num][5] - color_width < CSLeft_B))
    {
        ans++;
    }
    if ((color_world2[num][0] + color_width > CSRight_R && color_world2[num][0] - color_width < CSRight_R && color_world2[num][1] + color_width > CSRight_G && color_world2[num][1] - color_width < CSRight_G && color_world2[num][2] + color_width > CSRight_B && color_world2[num][2] - color_width < CSRight_B) || (color_world2[num][3] + color_width > CSRight_R && color_world2[num][3] - color_width < CSRight_R && color_world2[num][4] + color_width > CSRight_G && color_world2[num][4] - color_width < CSRight_G && color_world2[num][5] + color_width > CSRight_B && color_world2[num][5] - color_width < CSRight_B))
    {
        ans += 2;
    }
    return ans;
}
int IsOnYellowLine(void)
{
    return IsOnStuff(COLOR_YELLOW);
}
int IsOnRedObj(void)
{
    return IsOnStuff(COLOR_RED);
}
int IsOnCyanObj(void)
{
    return IsOnStuff(COLOR_CYAN);
}
int IsOnBlackObj(void)
{
    return IsOnStuff(COLOR_BLACK);
}
int IsOnSuperObj(void)
{
    return IsOnStuff(COLOR_SUPER_OBJ);
}
int IsOnBlueFloor(void)
{
    return IsOnStuff(COLOR_BLUE_FLOOR);
}
int IsOnDepositArea(void)
{
    return IsOnStuff(COLOR_DEPOSIT);
}
int IsOnSwampland(void)
{
    return IsOnStuff(COLOR_SWAMPLAND);
}
int IsOnInTrap(void) {
  return IsOnStuff(COLOR_IN_TRAP);
}

void Motor(int left, int right)
{
    WheelLeft = left;
    WheelRight = right;
    LED_1 = 0;
    MyState = 0;
    action = DEFIND;
}

#define CsBot_AI_C //DO NOT delete this line

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

DLL_EXPORT char *GetDebugInfo()
{
    static char info[1024];
    sprintf(info, "curcolor=%d;Duration=%d;SuperDuration=%d;bGameEnd=%d;CurAction=%d;CurGame=%d;SuperObj_Num=%d;SuperObj_X=%d;SuperObj_Y=%d;Teleport=%d;LoadedObjects=%d;US_Front=%d;US_Left=%d;US_Right=%d;CSLeft_R=%d;CSLeft_G=%d;CSLeft_B=%d;CSRight_R=%d;CSRight_G=%d;CSRight_B=%d;PositionX=%d;PositionY=%d;TM_State=%d;Compass=%d;Time=%d;WheelLeft=%d;WheelRight=%d;LED_1=%d;MyState=%d;",curcolor, Duration, SuperDuration, bGameEnd, CurAction, CurGame, SuperObj_Num, SuperObj_X, SuperObj_Y, Teleport, LoadedObjects, US_Front, US_Left, US_Right, CSLeft_R, CSLeft_G, CSLeft_B, CSRight_R, CSRight_G, CSRight_B, PositionX, PositionY, TM_State, Compass, Time, WheelLeft, WheelRight, LED_1, MyState);
    return info;
}

DLL_EXPORT char* GetTeamName()
{
     return "";
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

DLL_EXPORT void SetDataAI(volatile int *packet, volatile int *AI_IN)
{

    int sum = 0;

    US_Front = AI_IN[0];
    packet[0] = US_Front;
    sum += US_Front;
    US_Left = AI_IN[1];
    packet[1] = US_Left;
    sum += US_Left;
    US_Right = AI_IN[2];
    packet[2] = US_Right;
    sum += US_Right;
    CSLeft_R = AI_IN[3];
    packet[3] = CSLeft_R;
    sum += CSLeft_R;
    CSLeft_G = AI_IN[4];
    packet[4] = CSLeft_G;
    sum += CSLeft_G;
    CSLeft_B = AI_IN[5];
    packet[5] = CSLeft_B;
    sum += CSLeft_B;
    CSRight_R = AI_IN[6];
    packet[6] = CSRight_R;
    sum += CSRight_R;
    CSRight_G = AI_IN[7];
    packet[7] = CSRight_G;
    sum += CSRight_G;
    CSRight_B = AI_IN[8];
    packet[8] = CSRight_B;
    sum += CSRight_B;
    PositionX = AI_IN[9];
    packet[9] = PositionX;
    sum += PositionX;
    PositionY = AI_IN[10];
    packet[10] = PositionY;
    sum += PositionY;
    TM_State = AI_IN[11];
    packet[11] = TM_State;
    sum += TM_State;
    Compass = AI_IN[12];
    packet[12] = Compass;
    sum += Compass;
    Time = AI_IN[13];
    packet[13] = Time;
    sum += Time;
    packet[14] = sum;
}
DLL_EXPORT void GetCommand(int *AI_OUT)
{
    AI_OUT[0] = WheelLeft;
    AI_OUT[1] = WheelRight;
    AI_OUT[2] = LED_1;
    AI_OUT[3] = MyState;
}

DLL_EXPORT void OnTimer()
{
    switch (CurGame)
    {
    case 9:
        break;
    case 10:
        WheelLeft = 0;
        WheelRight = 0;
        LED_1 = 0;
        MyState = 0;
        break;
    case 0:
        Game0();
        break;

    default:
        break;
    }
}
