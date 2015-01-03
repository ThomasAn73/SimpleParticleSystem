#include "ClassEnvironment.hpp"
#include <iostream>
#include <stdlib.h>

const double g=9.84;
const double Friction=0.0001;

//***TYPEENVIRONMENT CLASS implementation*********************************************************************
//************************************************************************************************************

//---CONSTRUCTORS---------------------------------------------------------------------------------------------

TypeEnvironment::TypeEnvironment (TypeCartesian WindowSize, unsigned TileSize, double TimeStamp, bool UseParticles)
{
    //Set the private variables
    BoxSize=WindowSize;
    ShowAsParticles=UseParticles;
    CurrentTime=TimeStamp;

    //Establish a Grid size based on the suggested tile size. The objective is to make the grid tiles as square as possible
    double TileLeftOver;

    TileLeftOver=modf(WindowSize.X/TileSize, &GridSize.X);
    if (TileLeftOver > 0.5 ) GridSize.X ++;

    TileLeftOver=modf(WindowSize.Y/TileSize, &GridSize.Y);
    if (TileLeftOver > 0.5 ) GridSize.Y ++;

    std::cout<<"GridX="<<GridSize.X<<" GridY="<<GridSize.Y<<"\n";

    //Now that you have a grid size, establish a collisions Grid
    //Right now you have a NULL CollisionsGrid via the basic constructor so you got to initialize it to the correct size
    CollisionsGrid.Initialize(unsigned(GridSize.X), unsigned (GridSize.Y));

    //Load the dot Textures
    //sf::Texture OneTexture;
    //OneTexture.loadFromFile("dot10x10R.png"); DotTextures.push_back(OneTexture);
}
//---

void TypeEnvironment::setCurrentTime (double TimeStamp)
{ if(TimeStamp<0) {TimeStamp=0;} CurrentTime=TimeStamp; }

bool TypeEnvironment::AddNozzle (int SoManyParticles, double ParticleSize, double ParticleElast, double ParticleMass, double ParticleSpeed, double NozzlePixPosX, double NozzlePixPosY, double NozzleDir, double NozzleWidth)
{
    if (SoManyParticles==0 || ParticleSize<0 || ParticleSpeed<=0 || NozzleWidth<ParticleSize) return false;

    TypeVec NozzleVec;
    NozzleVec.setMagnitude(ParticleSpeed);
    NozzleVec.setDirection (NozzleDir);
    NozzleVec.setPixelOrigin (NozzlePixPosX, NozzlePixPosY);

    TypeNozzle OneHose;
    OneHose.SoMany=SoManyParticles;
    OneHose.ParticleRadius=ParticleSize/2;
    OneHose.ParticleElast=ParticleElast;
    OneHose.ParticleMass=ParticleMass;
    OneHose.PixWidth=NozzleWidth;
    OneHose.LastTime=CurrentTime;
    OneHose.NozzleVec=NozzleVec;

    Hoses.push_back(OneHose);
    return true;
}

void TypeEnvironment::AddParticle (double ParticleSize, double ParticleElast, double ParticleMass, double ParticleSpeed, double Dir, double PixPosX, double PixPosY)
{
    TypeBall OneParticle(ParticleSize/2,ParticleElast,ParticleMass);
    sf::Vertex ScreenPoint; ScreenPoint.color = sf::Color (255,255,255);

    TypeVec ParticleVelocity;

    ParticleVelocity.setMagnitude(ParticleSpeed);
    ParticleVelocity.setDirection(Dir);
    ParticleVelocity.setPixelOrigin(PixPosX,PixPosY);
    ParticleVelocity.setTimeStamp(CurrentTime);

    OneParticle.setVelocity (ParticleVelocity);
    TypeCartesian PixPosition=ParticleVelocity.getPixelOrigin();
    ScreenPoint.position = sf::Vector2f( PixPosition.X-ParticleSize/2, PixPosition.Y-ParticleSize/2);

    Particles.push_back(OneParticle);
    ScreenObjects.append(ScreenPoint);

    Population=Particles.size();
}

//Generates a row of particles at the nozzle edge
void TypeEnvironment::GenerateParticles ()
{
    if (Hoses.size()==0) return;
    TypeVec ParticleVelocity;

    unsigned h=0;
    while (h<Hoses.size())
    {
        double ParticleRadius=Hoses[h].ParticleRadius;
        double Elast=Hoses[h].ParticleElast;
        double Mass=Hoses[h].ParticleMass;
        double TimeDif=CurrentTime - Hoses[h].LastTime;
        double PixDistance=Hoses[h].NozzleVec.getMagnitude() * TimeDif; PixDistance=ParticleVelocity.ConvertToPixelDistance (PixDistance);

        if (PixDistance<ParticleRadius*2 || Hoses[h].SoMany==0) {h++; continue;} //Not ready to generate particles from this Hose

        int SoMany=Hoses[h].PixWidth / (2*ParticleRadius);
        if (SoMany>Hoses[h].SoMany) SoMany=Hoses[h].SoMany;

        //Create a new vector at the nozzle position and make it point up with magnitude = half the nozzle width
        //Get the position at the tip of this vector
        //Move this vector to that tip position and reverse its direction
        TypeVec HoseEdge;
        TypeCartesian AdjustedPos;
        HoseEdge.setMagnitude(HoseEdge.ConvertToMeterDistance(Hoses[h].PixWidth/2));
        HoseEdge.setDirection(Hoses[h].NozzleVec.getDirection() + 90);
        HoseEdge.setOrigin(Hoses[h].NozzleVec.getOrigin());
        AdjustedPos=HoseEdge.getTipCoordinates();
        HoseEdge.setOrigin(AdjustedPos);
        HoseEdge.setDirection (HoseEdge.getOpositeDir() );


        for (int i=0; i<SoMany; ++i)
        {
            TypeBall OneParticle(ParticleRadius,Elast,Mass);
            sf::Vertex ScreenPoint;

            ScreenPoint.color = sf::Color (255,255,255);

            double PixOffset = i*2.5*ParticleRadius;
            HoseEdge.setMagnitude(HoseEdge.ConvertToMeterDistance(PixOffset));
            AdjustedPos=HoseEdge.getTipCoordinates();

            ParticleVelocity = Hoses[h].NozzleVec;
            ParticleVelocity.setOrigin(AdjustedPos);
            ParticleVelocity.setTimeStamp(CurrentTime);

            //srand (time(NULL));
            double Jitter=rand()%10 - 5;
            ParticleVelocity.setDirection(ParticleVelocity.getDirection()+Jitter);

            OneParticle.setVelocity (ParticleVelocity);
            AdjustedPos=ParticleVelocity.getPixelOrigin();
            ScreenPoint.position = sf::Vector2f( AdjustedPos.X-ParticleRadius, AdjustedPos.Y-ParticleRadius);

            Particles.push_back(OneParticle);
            ScreenObjects.append(ScreenPoint);
        }

        Hoses[h].LastTime=CurrentTime;
        Hoses[h].SoMany=Hoses[h].SoMany - SoMany;   //Subtract the particle count we just made from the toDo pool

        if (Hoses[h].SoMany <=0) {Hoses.erase(Hoses.begin()+h); h--;} //If no particles left, delete the hose
        h++;

        Population=Particles.size();
    }
}

void TypeEnvironment::Show (sf::RenderWindow& ThisWindow)
{
    GenerateParticles(); //If the nozzles are not finished, keep pumping particles

    if (Population==0) return;

    for (int i=0, k=rand()%Population;i<Population;i++, k++) //start at a random spot in the particle array
    {
        if(k==Population) k=0;
        Kinematics(k, true);
        CheckBoundCollision (k);
        CheckInterRepel(k);
    }

    if (ShowAsParticles == true) {ThisWindow.draw(ScreenObjects);}
}

TypeCartesian TypeEnvironment::UpdateGridPosition(unsigned ThisOne)
{
    TypeCartesian NewGridIndex, OldGridIndex;
    OldGridIndex=Particles[ThisOne].getGridPosition();   //Get the last known location before you change it
    NewGridIndex=Particles[ThisOne].setGridPosition(BoxSize, GridSize); //Inform the particle itself of its own new grid coordinates

    //Update the collision grid (move particle from old index to a new grid index)
    if(OldGridIndex.X<0 || OldGridIndex.Y<0) CollisionsGrid.AddValueAt(unsigned(NewGridIndex.X),unsigned(NewGridIndex.Y),ThisOne);
    else CollisionsGrid.MoveDataValue(unsigned(OldGridIndex.X),unsigned(OldGridIndex.Y),unsigned(NewGridIndex.X),unsigned(NewGridIndex.Y),ThisOne);

    return NewGridIndex;
}

void TypeEnvironment::CheckInterRepel (unsigned ThisOne)
{
    TypeCartesian NewGridIndex=UpdateGridPosition (ThisOne);

    //Create an array of colliding particles by searching nine grid cells (the current cells and eight surrounding ones)
    std::vector<unsigned> ColSystem;

    for (int j=-1;j<=1;j++) //Grid Columns
    {
        for (int i=-1;i<=1;i++) //Grid Rows
        {   long CellPopulation=CollisionsGrid.getCellPopulation(NewGridIndex.X-i,NewGridIndex.Y-j);
            if (CellPopulation <=0) continue;
            for (long k=0;k<CellPopulation;k++) //Within a grid cell
            {
                unsigned ThatOne=CollisionsGrid.getValueFrom(NewGridIndex.X-i,NewGridIndex.Y-j,k);
                if (isCollided(ThisOne,ThatOne)==true) ColSystem.push_back(ThatOne);
            }
        }
    }

    if (ColSystem.size()==0) return; //Nothing to do

    for (unsigned i=0;i<ColSystem.size();i++)
    { long ThatOne=ColSystem[i]; ElasticRepel(ThisOne,ThatOne, ColSystem.size()); }

}

bool TypeEnvironment::isCollided (unsigned ThisOne, unsigned ThatOne)
{
    if (ThisOne==ThatOne) return false;

    TypeVec ThisVelocity=Particles[ThisOne].getVelocity();
    TypeVec ThatVelocity=Particles[ThatOne].getVelocity();
    TypeVec BridgeVec=ThisVelocity.getVectorToPoint(ThatVelocity);

    //Check angle of approach for velocity
    double BridgeAngle = BridgeVec.getIncidentAngle(ThisVelocity);
    double TailAngle = ThatVelocity.getIncidentAngle(ThisVelocity);
    if (BridgeAngle<270 && BridgeAngle>90 && TailAngle<270 && TailAngle>90) return false;

    //Check interference radius
    double PixDist=ThisVelocity.getPixelDistToVector (ThatVelocity); //This is 'r' in literature
    double MinPixDistance=Particles[ThisOne].getRadius()+Particles[ThatOne].getRadius(); //This the interaction radius 'h' in literature
    if (PixDist/MinPixDistance<1) { return true; } else {return false;}
}

void TypeEnvironment::ElasticRepel (unsigned ThisOne, unsigned ThatOne, unsigned NearPopulation)
{
    TypeVec AccelOne=Particles[ThisOne].getAcceleration(); double MassOne=Particles[ThisOne].getMass();
    TypeVec AccelTwo=Particles[ThatOne].getAcceleration(); double MassTwo=Particles[ThatOne].getMass();
    TypeVec NewAccel;

    TypeVec BridgeVec=AccelOne.getVectorToPoint(AccelTwo);
    //if (BridgeVec.getMagnitude()<0.01) BridgeVec.setDirection(AccelOne.getOpositeDir());

    double minPixDist=Particles[ThisOne].getRadius()+Particles[ThatOne].getRadius();
    double PixDist=AccelOne.getPixelDistToVector(AccelTwo); if (PixDist>minPixDist) return;

    double k=1.0-1.0/(2*NearPopulation);  //This should vary between 0.75 and 1.0 (anything less than one produces the initial pull effect)
    double pushPullAccel=200*g*pow(k-PixDist/minPixDist,5); //This is the collision Accel, then multiply by their mass proportions
    double Jitter=0.0; Jitter=(rand()%180 - 90)*(1-pow(PixDist/minPixDist,3)); //Jitter to pop out of a straight-line during squeeze

    //pushPullAccel=pushPullAccel*(0.5+0.5*PixDist/minPixDist);

    //Transfer of acceleration from particle one to particle two
    NewAccel.setPolarComponents((pushPullAccel)*MassOne/MassTwo,BridgeVec.getDirection()+Jitter);
    AccelTwo=AccelTwo+NewAccel;
    Particles[ThatOne].setAcceleration(AccelTwo);

    //Transfer of acceleration from particle two to particle one
    NewAccel.setPolarComponents((pushPullAccel)*MassTwo/MassOne,BridgeVec.getOpositeDir()+Jitter);
    AccelOne=AccelOne+NewAccel;
    Particles[ThisOne].setAcceleration(AccelOne);
}

bool TypeEnvironment::CheckBoundCollision (unsigned ThisOne)
{
    TypeVec BallVelocity=Particles[ThisOne].getVelocity();
    TypeVec BallAccel=Particles[ThisOne].getAcceleration();
    TypeCartesian BallPos=BallVelocity.getPixelOrigin();

    bool Collided=false, T=false, B=false, L=false, R=false;
    double Speed=BallVelocity.getMagnitude();
    double Dir=BallVelocity.getDirection();
    double PlaneAngle;
    double ObjRadius=Particles[ThisOne].getRadius();
    double Elast=Particles[ThisOne].getElasticity();

    if(BallPos.Y>BoxSize.Y-ObjRadius)   //Bottom Side
    { BallPos.Y=BoxSize.Y-ObjRadius; PlaneAngle=0.0; Collided=true; B=true; }
    else if (BallPos.Y<ObjRadius)       //Top Side
    { BallPos.Y=ObjRadius; PlaneAngle=0.0; Collided=true; T=true; }

    if (BallPos.X>BoxSize.X-ObjRadius)  //Right Side
    { BallPos.X=BoxSize.X-ObjRadius; PlaneAngle=90.0; Collided=true; R=true; }
    else if (BallPos.X<ObjRadius)       //Left Side
    { BallPos.X=ObjRadius; PlaneAngle=90.0; Collided=true; L=true; }

    if(Collided==true)
    {
        if (B==true && Dir>270) Dir=0; else if (B==true && Dir<270) Dir=180; else if (B==true && Dir==270) Dir=180*rand()%1;
        else if (T==true && Dir<90) Dir=0; else if (T==true && Dir>90) Dir=180; else if (T==true && Dir==90) Dir=180*rand()%1;
        else if (L==true && Dir>180) Dir=270; else if (L==true && Dir<180) Dir=90; else if (L==true && Dir==180) Dir=180*rand()%1+90;
        else if (R==true && Dir<90 && Dir>0) Dir=90; else if (R==true && Dir>270) Dir=270; else if (R==true && Dir==0) Dir=180*rand()%1+90;
        BallVelocity.setPolarComponents(Speed*(1-Friction), Dir);

        //BallVelocity.setPolarComponents(Speed, BallVelocity.CalcReflectionAngle(PlaneAngle));
        //BallVelocity.scaleCartesianComponents(1-Friction, Elast, PlaneAngle);
        BallVelocity.setPixelOrigin(BallPos);    //The position has been adjusted by the bounce tests

        Particles[ThisOne].setVelocity(BallVelocity);
        ScreenObjects[ThisOne].position = sf::Vector2f( BallPos.X, BallPos.Y);
    }

    return Collided;
}

void TypeEnvironment::Kinematics (unsigned ThisOne, bool UseGravity)
{
    double TimeDif=CurrentTime-Particles[ThisOne].getTimeStamp();
    TypeCartesian NewPos, NewVel, OldPos, OldVel, Accel;

    OldPos =Particles[ThisOne].getPosition();
    OldVel =Particles[ThisOne].getCartesianVel();
    Accel  =Particles[ThisOne].getCartesianAccel();                     //Get the previous accel before reset
    if(UseGravity==true) Accel.Y=Accel.Y-g;
    //if(ThisOne==2 && Particles[ThisOne].getAcceleration().getMagnitude()>g)std::cout<<"P["<<ThisOne<<"], has accel="<<Particles[ThisOne].getAcceleration().getMagnitude()<<", dir="<<Particles[ThisOne].getAcceleration().getDirection()<<"\n";

    //Adjust vertex color based on acceleration value
    AdjustDotColor (ThisOne);

    //This is in meters.
    NewPos.X=OldPos.X + OldVel.X*TimeDif + 0.5*Accel.X*TimeDif*TimeDif;
    NewPos.Y=OldPos.Y + OldVel.Y*TimeDif + 0.5*Accel.Y*TimeDif*TimeDif;

    NewVel.X=OldVel.X + Accel.X*TimeDif;
    NewVel.Y=OldVel.Y + Accel.Y*TimeDif;

    //Set new position
    Particles[ThisOne].setFromCartesian(NewVel,NewPos,CurrentTime);
    NewPos=Particles[ThisOne].getPixelPosition();
    ScreenObjects[ThisOne].position = sf::Vector2f( NewPos.X, NewPos.Y);

    //Reset particle accelaration
    Particles[ThisOne].setAcceleration(0,0);
}

void TypeEnvironment::AdjustDotColor (unsigned ThisOne)
{
    if (ThisOne>Particles.size()-1) return;

    double AccelMag=Particles[ThisOne].getAccelMag();
    double radius=Particles[ThisOne].getRadius();
    int TexIndex=0, TexColorOffset=0;

    if (AccelMag>=20*g) {TexColorOffset=0; ScreenObjects[ThisOne].color=sf::Color (255,255,0);}
    else if (AccelMag>=2*g && AccelMag<20*g) {TexColorOffset=1; ScreenObjects[ThisOne].color=sf::Color (0,255,0);}
    else {TexColorOffset=2; ScreenObjects[ThisOne].color=sf::Color (255,255,255);}

    if(radius<20) {TexIndex=0; ScreenObjects[ThisOne].texCoords= sf::Vector2f(5,5);}
    else if (radius>=20 && radius<40) {TexIndex=3; ScreenObjects[ThisOne].texCoords== sf::Vector2f(10,10);}
    else {TexIndex=6; ScreenObjects[ThisOne].texCoords== sf::Vector2f(20,20);}

    if (ShowAsParticles==false) {}
}
