#include "ClassEnvironment.hpp"
#include <SFML/Graphics.hpp>

int main ()
{
    using namespace sf;

    Event Sensed;                           //Create an object of type "Event" and give it a name: "Sensed"
    TypeCartesian wSize={800,400};
    Clock TheClock; double CurrentTime=0.0;
    int pSize=10;

    srand(time(NULL));

    RenderWindow MainWindow;                //Declare an sfml render window
    MainWindow.create(VideoMode(wSize.X,wSize.Y), "Collisions");

    //Create environment object
    TypeEnvironment FluidicSpace (wSize,pSize,0.0,true);

    //FluidicSpace.AddNozzle(200,pSize,0,1,1.5,400,100,0,pSize*2);
    //FluidicSpace.AddNozzle(300,pSize,0.1,1,3,400,50,-90,pSize*4);

    //FluidicSpace.AddParticle(pSize,0,1,0.2,0,500,280);
    //FluidicSpace.AddParticle(pSize,1,1,0,0,280,280);
    //FluidicSpace.AddParticle(pSize,1,1,0,0,305,280);
    //FluidicSpace.AddParticle(pSize,0,1,0,0,660,280);

    //FluidicSpace.AddParticle(pSize,0,1,0,0,700,280);
    //FluidicSpace.AddParticle(pSize,0,1,0,0,650,280);
    //FluidicSpace.AddParticle(pSize,0,1,0,0,600,280);

    TypeCartesian Pnum (6,30);
    for (int j=0;j<Pnum.Y;j++)
    {
        for (int i=0;i<Pnum.X;i++)
        { FluidicSpace.AddParticle(pSize,0,1,0,0,wSize.X/2-Pnum.X*pSize/2+i*(pSize-1),0+pSize/2+j*(pSize-1)); }
    }


    TheClock.restart();

    //Everything happens within this loop
    while (MainWindow.isOpen()==true)
    {
        CurrentTime=CurrentTime+0.0005;
        FluidicSpace.setCurrentTime(CurrentTime);

        //Functions that display objects on screen
        MainWindow.clear();
        FluidicSpace.Show(MainWindow);
        MainWindow.display();

        //Handle events
        while (MainWindow.pollEvent(Sensed)==true)
        {
            if (Sensed.type == Event::Closed) {MainWindow.close(); break;}
        }
    }

    return 0;
}
