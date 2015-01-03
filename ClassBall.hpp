#ifndef CuteBall0101110_h
#define CuteBall0101110_h

#include "ClassVector.hpp"
#include <vector>

class TypeBall
{
private:
//Private variables
    double   radius=2.0;
    double   Elasticity=0.9;
    double   Mass=1.0;              //Kg
    double   Density=0.0;           //Density at this particle

    TypeCartesian GridPosition;     //keeps record of current placement on a collision grid array
    TypeVec CurVelocity;            //mathematical vector; contains Magnitude, Direction, Origin, and TimeStamp
    TypeVec Acceleration;           //Acceleration vector m/s^2

//Private methods
    void setGridPosition (TypeCartesian GridPos);
    void setGridPosition (double GridX, double GridY);

public:
//Constructors
    TypeBall(double Radius=2.0, double Elast=0.9, double M=1.0, TypeVec CurVel = TypeVec() );

//set Methods
    void setElasticity (double Elast);
    void setMass (double BallMass);
    void setVelocity (TypeVec Velocity);
    void setAcceleration (double Magnitude, double DirectionDeg);
    void setAcceleration (TypeVec AccelVector);
    void setSpeed (double VelMagnitude);
    void setDirection (double VelDirection);
    void setPosition (TypeCartesian NewPos);
    void setPixelPosition (TypeCartesian PixPos);
    void setVelTimeStamp (double ThisTime);
    void setRadius (double NewRadius);
    void setFromCartesian (TypeCartesian NewVelocity, TypeCartesian NewPosition, double atTime=0.0);
    TypeCartesian setGridPosition (TypeCartesian WindowSize, TypeCartesian GridSize);

//get Methods
    double getRadius ();
    double getSpeed ();
    double getElasticity ();
    double getMass ();
    double getTimeStamp();
    double getDirection ();
    double getAccelMag ();
    double getAccelDir ();
    TypeCartesian getPosition();
    TypeCartesian getPixelPosition();
    TypeCartesian getGridPosition();
    TypeCartesian getCartesianVel();
    TypeCartesian getCartesianAccel();
    TypeVec getVelocity ();
    TypeVec getAcceleration();

    TypeVec CalcDistanceToPoint (TypeCartesian Point);
    TypeCartesian PixelPosition (TypeCartesian MeterPos);
    TypeCartesian MeterPosition (TypeCartesian PixPos);
    double PixelDistance (double MeterDist);
    double MeterDistance (double PixelDist);
    void showData ();
};

#endif // CuteBall0101110_h
