#ifndef ClassVector_0101110_h
#define ClassVector_0101110_h

struct TypeCartesian {
    double X; double Y; double Z;
    TypeCartesian(double x=0.0, double y=0.0, double z=0.0){X=x; Y=y; Z=z;} //Struct Constructor
};

struct TypePolar {
    double Magn; double Dirct;
    TypePolar(double m=0.0, double d=0.0){Magn=m; Dirct=d;} //Struct Constructor
};

class TypeVec
{
private:
    //Private variables
    double TimeStamp=0.0;           //since a force last acted on the object
    TypeCartesian VecOrigin;        //position of vector origin
    TypeCartesian VecCompt;         //XY components of vector velocity
    TypePolar VecPolar;             //Polar components of velocity (Direction in degrees)

    //Private Methods
    double ToRad (double Deg) const;// the 'const' at the end means this function promises not to change the private variables
    double ToDeg (double Rad) const;

public:
    //Consrt/Destr
    TypeVec();
    TypeVec (double Mag, double Dir, double Xpos, double Ypos, double NewTimeStamp=0.0);
    ~TypeVec();

    //METHODS
    TypePolar CalcPolarComponents (double Xcomp, double Ycomp) const;
    TypeCartesian CalcCartesianFromPolar(double Magnitude, double Direction) const;
    TypeCartesian CalcCartesianFromRelativeCart(double Xcomp, double Ycomp, double DatumAngle=0) const;
    TypeCartesian ConvertToPixelPosition (TypeCartesian MeterPos) const;
    TypeCartesian ConvertToMeterPosition (TypeCartesian PixPos) const;
    double ConvertToPixelDistance (double MeterDist) const;
    double ConvertToMeterDistance (double PixelDist) const;
    double CleanDir (double ThisDir, bool isPlane=false) const;
    double CalcReflectionAngle (double PlaneAngle) const;
    double CalcDirection (TypeCartesian Component) const;

    //SET methods
    void setMagnitude (double Mag);
    void setDirection (double Dir);
    void setPolarComponents (TypePolar FromThis);
    void setPolarComponents (TypeVec FromThis);
    void setPolarComponents (double Magnitude, double DirectionDeg);
    void setCartesianComponents (double Xaxis, double Yaxis, double DatumAngle=0);
    void setCartesianComponents (TypeCartesian VectorComponents, double DatumAngle=0);
    void setFromCartesian (TypeCartesian NewXYcomponents, TypeCartesian NewXYposition, double atTime=0.0);
    void setTimeStamp (double ThisTime);
    void setOrigin (TypeCartesian Origin);
    void setOrigin (double Xpos, double Ypos);
    void setPixelOrigin (TypeCartesian PixOrigin);
    void setPixelOrigin (double Xpos, double Ypos);
    void scaleCartesianComponents (double Xscalar, double Yscalar, double DatumAngle);

    //GET methods
    double getMagnitude () const;
    double getDirection () const;
    double getRadsDirection () const;
    double getTimeStamp () const;
    double getOpositeDir () const;
    double getDirectionToPoint (TypeCartesian DestPoint) const;
    double getDistanceToPoint (TypeCartesian DestPoint) const;
    double getPixelDistToPoint (TypeCartesian DestPoint) const;
    double getDistanceToVector (TypeVec DestVect) const;
    double getPixelDistToVector (TypeVec DestVect) const;
    double getIncidentAngle (TypeVec OtherVect) const;
    TypeVec getVectorToPoint (TypeCartesian DestPoint) const;
    TypeVec getVectorToPoint (TypeVec DestVec) const;
    TypeVec getResultantFrom (TypeCartesian VecComponents) const;
    TypeVec getResultantFrom (double Xcomp, double Ycomp) const;
    TypeCartesian getCartesianComponents() const;
    TypeCartesian getVelAboutRotatedAxis(double RotationAngle) const;
    TypeCartesian getProjectedComponents (TypeVec ofThis) const;
    TypeCartesian getOrigin () const;
    TypeCartesian getPixelOrigin () const;
    TypeCartesian getTipCoordinates () const;
    TypeCartesian getTipPixelCoordinates () const;

    //Operator overloading methods -> for vector arithmitic
    TypeVec operator+ (const TypeVec& ThatVec) const;
    TypeVec operator- (const TypeVec& ThatVec) const;
    TypeVec operator* (const double& scalar) const;
    TypeVec operator/ (const double& scalar) const;
    //bool operator> (TypeVec& ThatVec);
    //bool operator< (TypeVec& ThatVec);
    //bool operator== (TypeVec& ThatVec);
    //TypeVec operator* (TypeVec& ThatVec); //Dot product I suppose
};

#endif
