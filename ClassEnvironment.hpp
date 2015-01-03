#ifndef ClassEnvironment_0101110_h
#define ClassEnvironment_0101110_h

#include "ClassBall.hpp"
#include "ClassListsMatrix.hpp"
#include <SFML/Graphics.hpp>
#include <vector>   //standard library vector array

class TypeEnvironment
{
private:
//Private Variables
    int Population=0;
    double CurrentTime;
    bool ShowAsParticles=true;

    std::vector <TypeBall> Particles;   //Array of all particles in the environment
    sf::VertexArray ScreenObjects;      //Array of screen manifestations of the particles
    std::vector <sf::Texture> DotTextures;

    struct TypeNozzle
    {
        int SoMany;
        double ParticleRadius;
        double ParticleElast;
        double ParticleMass;
        double PixWidth;
        double LastTime;
        TypeVec NozzleVec;          //MeterPosition, Direction, particle speed
    };
    std::vector <TypeNozzle> Hoses;

    TypeCartesian BoxSize;          //Size of the conataining window
    TypeCartesian GridSize;         //This is not an array index, it's a size (the index would be GridSize-1)
    TypeListsMatrix CollisionsGrid; //All objects register here to detect collisions

//Private methods
    bool isCollided (unsigned ThisOne, unsigned ThatOne);
    bool CheckBoundCollision (unsigned ParticleIndex);
    //void CheckInterCollisions (unsigned ParticleIndex);
    void CheckInterRepel (unsigned ThisOne);
    void ElasticRepel (unsigned ThisOne, unsigned ThatPne, unsigned NearPopulation);
    void Kinematics (unsigned ParticleIndex, bool UseGravity=true);
    void GenerateParticles (); //Generates a row of particles at the nozzle edge
    void ConserveMomentum (TypeBall& FirstOne, TypeBall& SecondOne, bool NudgeAway);
    void AdjustDotColor (unsigned ThisOne);
    TypeCartesian UpdateGridPosition(unsigned ThisOne);

public:
//Constructors
    TypeEnvironment (TypeCartesian WindowSize, unsigned TileSize, double TimeStamp=0.0, bool UseParticles=true);

//Methods
    bool AddNozzle (int SoManyParticles, double ParticleSize, double ParticleElast, double ParticleMass, double ParticleSpeed, double NozzlePixPosX, double NozzlePixPosY, double NozzleDir, double NozzleWidth);
    void AddParticle (double ParticleSize, double ParticleElast, double ParticleMass, double ParticleSpeed, double Dir, double PixPosX, double PixPosY);
    void setCurrentTime (double CurrentTime);
    void Show (sf::RenderWindow& ThisWindow);
};

#endif // ClassEnvironment
