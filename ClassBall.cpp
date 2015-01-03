#include "ClassBall.hpp"
#include <cmath>
#include <iostream>

//***TYPEBALL CLASS implementation****************************************************************************
//************************************************************************************************************

//---CONSTRUCTORS---------------------------------------------------------------------------------------------
TypeBall::TypeBall (double CirRadius, double Elast, double M, TypeVec CurVel)
{
    if (CirRadius<=0) CirRadius=2;
    if (M<=0) M=1.0;
    if (Elast<0) Elast=0;
    Mass=M;
    Elasticity=Elast;
    radius=CirRadius;
    CurVelocity=CurVel; //Memberwise copy; I hope.
    GridPosition.X=-1, GridPosition.Y=-1; //We don't know the grid size or window size yet
}

//---GET methods------------------------------------------------------------------------------------
double TypeBall::getRadius () { return radius; }

double TypeBall::getSpeed () { return CurVelocity.getMagnitude(); }

double TypeBall::getElasticity () {return Elasticity;}

double TypeBall::getMass () {return Mass;}

double TypeBall::getDirection () { return CurVelocity.getDirection(); }

double TypeBall::getTimeStamp() { return CurVelocity.getTimeStamp(); }

double TypeBall::getAccelMag () { return Acceleration.getMagnitude(); }

double TypeBall::getAccelDir () { return Acceleration.getDirection(); }

TypeVec TypeBall::getVelocity () { return CurVelocity; }

TypeVec TypeBall::getAcceleration() { return Acceleration; }

TypeCartesian TypeBall::getCartesianVel() { return CurVelocity.getCartesianComponents(); }

TypeCartesian TypeBall::getCartesianAccel() { return Acceleration.getCartesianComponents(); }

TypeCartesian TypeBall::getPosition() { return CurVelocity.getOrigin(); }

TypeCartesian TypeBall::getPixelPosition() { return CurVelocity.getPixelOrigin(); }

TypeCartesian TypeBall::getGridPosition() { return GridPosition; }

//---SET methods--------------------------------------------------------------------------------------------
void TypeBall::setPixelPosition (TypeCartesian PixPos)
{ setPosition(MeterPosition(PixPos)); }

void TypeBall::setPosition (TypeCartesian NewPos)
{ CurVelocity.setOrigin(NewPos); Acceleration.setOrigin(NewPos);} //Store position in meters

void TypeBall::setGridPosition (TypeCartesian GridPos)
{ GridPosition.X=GridPos.X; GridPosition.Y=GridPos.Y; }

void TypeBall::setGridPosition (double GridX, double GridY)
{ GridPosition.X=GridX; GridPosition.Y=GridY; }

void TypeBall::setVelTimeStamp (double ThisTime)
{ if(ThisTime<0) ThisTime=0; CurVelocity.setTimeStamp(ThisTime); }

TypeCartesian TypeBall::setGridPosition (TypeCartesian WindowSize, TypeCartesian GridSize)
{
    if (GridSize.X<=0 || GridSize.Y <=0) return GridPosition; //We don't want any divide by zero situations
    TypeCartesian PixPos = CurVelocity.getPixelOrigin();
    TypeCartesian Tile (WindowSize.X/GridSize.X, WindowSize.Y/GridSize.Y); //using the struct constructor
    modf(PixPos.X/Tile.X, &GridPosition.X); //Get the integer part of a double (modf is part of cmath)
    modf(PixPos.Y/Tile.Y, &GridPosition.Y); //Get the integer part of a double

    //Remember that GridPosition must be between 0 to GridSize-1
    //In the unlikely case that the object pixel position falls exactly at the very last Window pixel,
    //causing the GridPosition to equal GridSize (instead of GridSize-1)
    if (GridPosition.X > GridSize.X-1) GridPosition.X = GridSize.X-1;
    if (GridPosition.Y > GridSize.Y-1) GridPosition.Y = GridSize.Y-1;

    return GridPosition;
}

void TypeBall::setFromCartesian (TypeCartesian VelocityComponents, TypeCartesian NewPosition, double atTime)
{ CurVelocity.setFromCartesian(VelocityComponents, NewPosition, atTime); Acceleration.setOrigin(NewPosition); }

void TypeBall::setElasticity (double Elast)
{ if (Elast<0) {Elast=1;} Elasticity=Elast;}

void TypeBall::setMass (double BallMass)
{ if (BallMass<0) {BallMass=1;} Mass=BallMass;}

void TypeBall::setSpeed (double VelMagnitude)
{ CurVelocity.setMagnitude(VelMagnitude); }

void TypeBall::setDirection (double VelDirection)
{ CurVelocity.setDirection(VelDirection); }

void TypeBall::setVelocity (TypeVec Velocity)
{ CurVelocity=Velocity; Acceleration.setOrigin(Velocity.getOrigin());}

void TypeBall::setAcceleration (double Magnitude, double DirectionDeg)
{
    Acceleration.setMagnitude(Magnitude);
    Acceleration.setDirection(DirectionDeg);
}

void TypeBall::setAcceleration (TypeVec AccelVector)
{
    Acceleration=AccelVector;
    CurVelocity.setOrigin(Acceleration.getOrigin());
}

void TypeBall::setRadius (double NewRadius)
{ if(NewRadius>=2) {radius=NewRadius;} else {radius=2;} }

//---general methods---------------------------------------------------------------------------------
double TypeBall::PixelDistance (double MeterDist) { return CurVelocity.ConvertToPixelDistance(MeterDist); }

double TypeBall::MeterDistance (double PixelDist) { return CurVelocity.ConvertToMeterDistance(PixelDist); }

TypeCartesian TypeBall::PixelPosition (TypeCartesian MeterPos)
{
    TypeCartesian PixelPos;
    PixelPos.X=PixelDistance (MeterPos.X);
    PixelPos.Y=-PixelDistance (MeterPos.Y);

    return PixelPos;
}

TypeCartesian TypeBall::MeterPosition (TypeCartesian PixPos)
{
    TypeCartesian MetPos;

    MetPos.X=MeterDistance(PixPos.X);
    MetPos.Y=-MeterDistance(PixPos.Y);;

    return MetPos;
}

TypeVec TypeBall::CalcDistanceToPoint (TypeCartesian Point)
{ return CurVelocity.getVectorToPoint(Point); }

void TypeBall::showData()
{
    TypeCartesian Origin=CurVelocity.getOrigin();
    using namespace std;
    cout<<"R:"<<radius<<" Vel:"<<CurVelocity.getMagnitude()<<" Dir:"<<CurVelocity.getDirection();
    cout<<" Xpos:"<<Origin.X<<" Ypos:"<<Origin.Y<<endl;

}
