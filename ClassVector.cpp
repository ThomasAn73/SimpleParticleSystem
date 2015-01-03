#include "ClassVector.hpp"
#include <cmath>
//#include <iostream>

#define pi 3.141592654

//***TYPEVEC CLASS implementation*****************************************************************************
//************************************************************************************************************

//---CONSTRUCTORS---------------------------------------------------------------------------------------------
TypeVec::TypeVec () {TimeStamp=0.0;}

TypeVec::TypeVec (double Mag, double Dir, double Xpos, double Ypos, double atTime)
{
    setPolarComponents(Mag,Dir);  //This will update the cartesian velocity as well
    setTimeStamp(atTime);
    setOrigin(Xpos, Ypos);
}

TypeVec::~TypeVec () {}


//---MEMBER functions-----------------------------------------------------------------------------------------

double TypeVec::ToRad (double Deg) const
{ return 2.0 * pi * Deg/360.0; }

double TypeVec::ToDeg (double Rad) const
{ return 360.0 * Rad / (2.0*pi); }

TypeCartesian TypeVec::ConvertToPixelPosition (TypeCartesian MeterPos) const
{
    TypeCartesian PixelPos;
    PixelPos.X=ConvertToPixelDistance (MeterPos.X);
    PixelPos.Y=-ConvertToPixelDistance (MeterPos.Y);

    return PixelPos;
}

TypeCartesian TypeVec::ConvertToMeterPosition (TypeCartesian PixPos) const
{
    TypeCartesian MetPos;

    MetPos.X=ConvertToMeterDistance(PixPos.X);
    MetPos.Y=-ConvertToMeterDistance(PixPos.Y);;

    return MetPos;
}

double TypeVec::ConvertToPixelDistance (double MeterDist) const
{ const double PixPerMeter=5*(100/2.54); return MeterDist*PixPerMeter; }

double TypeVec::ConvertToMeterDistance (double PixelDist) const
{ const double PixPerMeter=5*(100/2.54); return PixelDist/PixPerMeter; }

double TypeVec::CleanDir (double ThisDir, bool isPlane) const
{
    if (std::abs(ThisDir)>=360) {ThisDir=fmod(ThisDir,360.0);} //Eliminate large positive angles
    else if (ThisDir<0) {ThisDir=fmod(ThisDir,360.0)+360.0;}   //Eliminate large negative angles
    if (isPlane==true && ThisDir>=180) {ThisDir=ThisDir-180;}

    //if ((ThisDir<(0.0+e) && ThisDir>(0.0-e)) || (ThisDir<(360.0+e) && ThisDir>(360.0-e)) ) ThisDir=0.0;

    return ThisDir;
}

double TypeVec::CalcReflectionAngle (double PlaneAngle) const
{
    double IncidentAngle, Result;

    PlaneAngle=CleanDir(PlaneAngle, true);
    IncidentAngle=VecPolar.Dirct-PlaneAngle;

    //the Dirct angle is always in reference to horizontal
    Result=-IncidentAngle + PlaneAngle;
    Result=CleanDir(Result);
    //std::cout<<"oldDir:"<<IncidentAngle<<" NewDir:"<<Result<<"\n";
    return Result;
}

//Direction angle in degrees from cartesian components
double TypeVec::CalcDirection (TypeCartesian Component) const
{
    double Result;

    if (Component.X==0 && Component.Y>=0) {Result=acos(0.0);}
    else if (Component.X==0 && Component.Y<=0) {Result=-acos(0.0);}
    else if (Component.X>0 && Component.Y>=0) Result=atan(Component.Y/Component.X);
    else if (Component.X<0 && Component.Y<=0) Result=atan(Component.Y/Component.X)+ToRad(180);
    else if (Component.X<0 && Component.Y>=0) Result=atan(Component.Y/Component.X)+ToRad(180);
    else Result=atan(Component.Y/Component.X);

    Result=ToDeg(Result);

    return CleanDir(Result);
}

TypeCartesian TypeVec::CalcCartesianFromRelativeCart(double Xcomp, double Ycomp, double DatumAngle) const
{
    if (DatumAngle<0 || DatumAngle>360) DatumAngle=CleanDir(DatumAngle);

    TypeCartesian ProjectedComp;
    if (DatumAngle==0) {ProjectedComp.X=Xcomp; ProjectedComp.Y=Ycomp; return ProjectedComp;}

    TypeCartesian XcompProjection, YcompProjection;
    XcompProjection.X=Xcomp*cos(ToRad(DatumAngle));
    XcompProjection.Y=Xcomp*sin(ToRad(DatumAngle));

    YcompProjection.X=Ycomp*sin(ToRad(DatumAngle));
    YcompProjection.Y=Ycomp*cos(ToRad(DatumAngle));

    ProjectedComp.X=XcompProjection.X+YcompProjection.X;
    ProjectedComp.Y=XcompProjection.Y+YcompProjection.Y;

    return ProjectedComp;
}

TypeCartesian TypeVec::CalcCartesianFromPolar(double Magnitude, double Direction) const
{
    if (Direction<0 || Direction>360) Direction=CleanDir(Direction);
    TypeCartesian CartComp;
    CartComp.X=cos(ToRad(Direction)) * Magnitude;
    CartComp.Y=sin(ToRad(Direction)) * Magnitude;
    return CartComp;
}

TypePolar TypeVec::CalcPolarComponents (double Xcomp, double Ycomp) const
{
    TypePolar PolarComp;
    TypeCartesian CartComp (Xcomp, Ycomp);
    PolarComp.Magn=sqrt(Xcomp*Xcomp + Ycomp*Ycomp);
    PolarComp.Dirct=CalcDirection(CartComp);
    return PolarComp;
}

//---SET methods----------------------------------------------------------------------------------------------
void TypeVec::setOrigin (TypeCartesian Origin) { VecOrigin=Origin; }

void TypeVec::setOrigin (double Xpos, double Ypos) { VecOrigin.X=Xpos; VecOrigin.Y=Ypos; }

void TypeVec::setPixelOrigin (double Xpos, double Ypos)
{
    TypeCartesian PixOrigin; PixOrigin.X=Xpos; PixOrigin.Y=Ypos;
    setPixelOrigin(PixOrigin);
}

void TypeVec::setPixelOrigin (TypeCartesian PixOrigin)
{
    TypeCartesian MetOrigin=ConvertToMeterPosition(PixOrigin);
    setOrigin(MetOrigin);
}

void TypeVec::setFromCartesian (TypeCartesian VectorComponents, TypeCartesian NewPosition, double NewTimeStamp)
{
    setCartesianComponents(VectorComponents);
    VecOrigin=NewPosition;
    TimeStamp=NewTimeStamp;
}

void TypeVec::setTimeStamp (double ThisTime) {if (ThisTime>=0) TimeStamp=ThisTime; else TimeStamp=0;}

void TypeVec::setMagnitude (double Mag)
{
    if (Mag>=0) {VecPolar.Magn=Mag;}
    else { VecPolar.Magn=-Mag; VecPolar.Dirct=getOpositeDir(); }
    VecCompt=CalcCartesianFromPolar(VecPolar.Magn, VecPolar.Dirct);
}

void TypeVec::setDirection (double Dir)
{
    VecPolar.Dirct=CleanDir(Dir);
    VecCompt=CalcCartesianFromPolar(VecPolar.Magn, VecPolar.Dirct);
}

void TypeVec::setPolarComponents (TypePolar FromThis)
{ setPolarComponents(FromThis.Magn, FromThis.Dirct); }

void TypeVec::setPolarComponents (TypeVec FromThis)
{ setPolarComponents(FromThis.getMagnitude(), FromThis.getDirection()); }

void TypeVec::setPolarComponents (double Magnitude, double DirectionDeg)
{
    VecPolar.Dirct=CleanDir(DirectionDeg);
    setMagnitude(Magnitude);
    VecCompt=CalcCartesianFromPolar(VecPolar.Magn, VecPolar.Dirct);
}

void TypeVec::setCartesianComponents (TypeCartesian VectorComponents, double DatumAngle)
{ setCartesianComponents(VectorComponents.X, VectorComponents.Y, DatumAngle); }

void TypeVec::setCartesianComponents (double Xcomp, double Ycomp, double DatumAngle)
{
    VecCompt=CalcCartesianFromRelativeCart(Xcomp,Ycomp,DatumAngle);
    VecPolar=CalcPolarComponents(VecCompt.X, VecCompt.Y);
}

void TypeVec::scaleCartesianComponents (double Xscalar, double Yscalar, double DatumAngle)
{
    TypeCartesian ProjectedScalar=CalcCartesianFromRelativeCart(Xscalar,Yscalar,DatumAngle);

    VecCompt.X=VecCompt.X * ProjectedScalar.X;
    VecCompt.Y=VecCompt.Y * ProjectedScalar.Y;

    VecPolar=CalcPolarComponents(VecCompt.X, VecCompt.Y);
}

//---GET methods-------------------------------------------------------------------------------------------
double TypeVec::getMagnitude () const { return VecPolar.Magn; }

double TypeVec::getDirection () const { return VecPolar.Dirct; }

double TypeVec::getTimeStamp () const {return TimeStamp;}

double TypeVec::getRadsDirection () const { return ToRad(VecPolar.Dirct); }

TypeCartesian TypeVec::getOrigin () const { return VecOrigin; }

TypeCartesian TypeVec::getPixelOrigin() const
{
    TypeCartesian MetOrigin=VecOrigin;
    return ConvertToPixelPosition(MetOrigin);
}

double TypeVec::getIncidentAngle (TypeVec OtherVect) const
{ return CleanDir( VecPolar.Dirct - OtherVect.VecPolar.Dirct ); }

TypeCartesian TypeVec::getCartesianComponents() const
{return VecCompt; }

TypeCartesian TypeVec::getTipCoordinates () const
{
    TypeCartesian Result=VecCompt;
    Result.X=Result.X + VecOrigin.X;
    Result.Y=Result.Y + VecOrigin.Y;
    return Result;
}

TypeCartesian TypeVec::getTipPixelCoordinates () const
{ return ConvertToPixelPosition(getTipCoordinates ()); }

//Direction angle in degrees; given the current origin point and a destination point
double TypeVec::getDirectionToPoint (TypeCartesian DestPoint) const
{ return getVectorToPoint(DestPoint).getDirection(); }

double TypeVec::getPixelDistToVector (TypeVec DestVect) const
{ return ConvertToPixelDistance ( getDistanceToPoint( DestVect.getOrigin() ) ); }

double TypeVec::getDistanceToVector (TypeVec DestVect) const
{ return getDistanceToPoint( DestVect.getOrigin() ); }

double TypeVec::getPixelDistToPoint (TypeCartesian DestPoint) const
{ return ConvertToPixelDistance( getVectorToPoint(DestPoint).getMagnitude() ); }

double TypeVec::getDistanceToPoint (TypeCartesian DestPoint) const
{ return getVectorToPoint(DestPoint).getMagnitude(); }

TypeVec TypeVec::getVectorToPoint (TypeCartesian DestPoint) const
{
    TypeVec ResultVec;
    double ResultDistance, Direction;
    TypeCartesian Distance;

    Distance.X=DestPoint.X-VecOrigin.X;
    Distance.Y=DestPoint.Y-VecOrigin.Y;

    ResultDistance=sqrt(Distance.X*Distance.X+Distance.Y*Distance.Y);
    Direction=CalcDirection(Distance);

    ResultVec.setDirection(Direction);
    ResultVec.setMagnitude(ResultDistance);
    ResultVec.setOrigin(VecOrigin);

    return ResultVec;
}

TypeVec TypeVec::getResultantFrom (TypeCartesian VecComponents) const
{ return getResultantFrom(VecComponents.X,VecComponents.Y); }

TypeVec TypeVec::getResultantFrom (double Xcomp, double Ycomp) const
{
    TypePolar Relative=CalcPolarComponents(Xcomp, Ycomp);
    TypeVec Result;
    Result.setPolarComponents(Relative.Magn,Relative.Dirct+VecPolar.Dirct);
    Result.setOrigin(VecOrigin);
    return Result;
}

TypeCartesian TypeVec::getProjectedComponents (TypeVec ofThis) const
{ return CalcCartesianFromPolar(ofThis.getMagnitude(),ofThis.getDirection()-VecPolar.Dirct); }

TypeVec TypeVec::getVectorToPoint (TypeVec DestVec) const
{
    TypeCartesian ToPoint=DestVec.getOrigin();
    return getVectorToPoint(ToPoint);
}

double TypeVec::getOpositeDir() const
{
    double Result;
    Result=VecPolar.Dirct+180.0;
    if (std::abs(Result)>360) Result=fmod(Result,360.0);

    return Result;
}

TypeVec TypeVec::operator+ (const TypeVec& ThatVec) const
{
    TypeVec ResultVec;
    TypeCartesian ThisXYcomp=getCartesianComponents();
    TypeCartesian ThatXYcomp=ThatVec.getCartesianComponents();
    TypeCartesian NewXYcomp;

    NewXYcomp.X=ThisXYcomp.X+ThatXYcomp.X;
    NewXYcomp.Y=ThisXYcomp.Y+ThatXYcomp.Y;

    ResultVec.setFromCartesian(NewXYcomp, VecOrigin, TimeStamp);
    return ResultVec;
}

TypeVec TypeVec::operator- (const TypeVec& ThatVec) const
{
    TypeVec ResultVec;
    TypeCartesian ThisXYcomp=getCartesianComponents();
    TypeCartesian ThatXYcomp=ThatVec.getCartesianComponents();
    TypeCartesian NewXYcomp;

    NewXYcomp.X=ThisXYcomp.X-ThatXYcomp.X;
    NewXYcomp.Y=ThisXYcomp.Y-ThatXYcomp.Y;

    ResultVec.setFromCartesian(NewXYcomp, VecOrigin, TimeStamp);
    return ResultVec;
}

TypeVec TypeVec::operator* (const double& scalar) const
{
    TypeVec ResultVec;
    ResultVec.setPolarComponents( VecPolar.Magn * scalar, VecPolar.Dirct );
    ResultVec.setOrigin(VecOrigin);
    ResultVec.setTimeStamp(TimeStamp);
    return ResultVec;
}

TypeVec TypeVec::operator/ (const double& scalar) const
{
    TypeVec ResultVec;
    ResultVec.setPolarComponents( VecPolar.Magn / scalar, VecPolar.Dirct );
    ResultVec.setOrigin(VecOrigin);
    ResultVec.setTimeStamp(TimeStamp);
    return ResultVec;
}
