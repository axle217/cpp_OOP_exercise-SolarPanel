/*
Objects interact in many different ways.
For instance objects can be passed at function/method arguments.
When this happens they can be manipulated just as POD(Plain Old Data i.e. int, double) arguments.
Object can be also used as a return type.
In all these situations one needs to remember that objects tend to be bigger (use more memory)
and we tend to use a cheap way of parring them around (i.e. take arguments by a const reference rather than a value).

This exercise is about how object are used to construct larger systems of objects (a model of the "world").
The SolarPanel class example is reused here.

We want to construct SolarPlant using them and model how much energy they can produce during the day.
We will use a simplified model in which the panel produces maximum energy when the light rays are exactly perpendicular to it and none when it is parallel.
That is the power = max power * cos (incidence angle). However the details of the calculation are not relevant here and we will have 1D world (only one angle will describe the position of the Sun).

Objects are related to each other by one of the following relations:
 - belongs to
 - uses
 - is kind of (modeled via inheritance)

In our "world" model we want to have 4 classes.
 - PanelSetup that will describe orientation of the SolarPanel (will be in relation "belongs to" with SolarPanel)
 - SolarPlant that will contain many PanelSetups (again modeling "belongs to")
 - the LightSource (the Sun) which will describe position of the light source (SolarPlant will "use" it to calculate power output)

The skill to project reality into classes is kind of an art.
It does not come naturally and requires a bit of experience.
A very useful principle to follow when building objetc system is the "Single Responsibility Principle".
That is one type of objects/class should be responsible for one functionality.
Last, but not least, sometimes there is no obvious way to do that.

*/
#include <iostream>
#include <cmath>

using namespace std;

constexpr double pi = 3.1415;

class SolarPanel {
public:
    SolarPanel(int dimX, int dimY)
        : m_dimx(dimX), m_dimy(dimY) {}
    inline double dimXinCM() const { return m_dimx * oneElementX; }
    inline double dimYinCM() const { return m_dimy * oneElementY; }
    inline double areainCM2() const { return dimXinCM() * dimYinCM(); }
    inline double maxPowerinW() const { return m_dimx * m_dimy * oneElementPowerinW; }
    inline void shrinkXto(int nelements) { m_dimx = nelements; }
    inline void shrinkYto(int nelements) { m_dimy = nelements; }

private:
    constexpr static float oneElementX = 6; // it is identical to what was in earlier exercise but with slightly more modern syntax
    constexpr static float oneElementY = 10;
    constexpr static float oneElementPowerinW = 15; // that many Watts of energy can each panel element produce at most

    int m_dimx;
    int m_dimy;
};


class PanelSetup {
public:

    PanelSetup(double angle = 0, const SolarPanel& panel = SolarPanel(20, 30))
        : m_orientationAngle(angle), m_panel(panel) {}; // here the default arguments are used to be able to construct the PanelSetup w/o any arguments if needed
        //PanelSetup(): m_orientationAngle(0), m_panel(SolarPanel(20, 30)) {} ;

    double currentPower(double angleInRadians) const {
        double curPow = std::cos(angleInRadians) > 0 ? m_panel.maxPowerinW() * std::cos(angleInRadians) : 0;
        return curPow;
    };
    // fraction of power produced compared to max
    double efficiency(double angleInRadians) const {
        double eff = std::cos(angleInRadians) > 0 ? 100 * currentPower(angleInRadians) / m_panel.maxPowerinW() : 0;
        return eff;
    };
    double getAngle() const { return m_orientationAngle; };
    double setAngle(double newangleInRadians) { return m_orientationAngle = newangleInRadians; };
    // IMPORTANT!! const SolarPanel& getPanel() const { return m_panel; } can't be modified
    SolarPanel& getPanel()  { return m_panel; } // add reference (&) to make it modifiable, otherwise it's just copying m_panel
    void setNPanel(int nx, int ny) {
        m_panel.shrinkXto(nx);  m_panel.shrinkYto(ny);
        cout<<m_panel.areainCM2() << endl;
    }
private:
    double m_orientationAngle;
    SolarPanel m_panel;
};

// Exercise 1
// Complete the definition of the PanelSetup class.
// The only place where there is some logic is in the currenPower
// To be able to use the cosine function include <cmath> (at the top of the file) and call it with std::cos( angleInRadians )
// Here you would need to choose angle definition.
// and test it by creating a specific object and test if indeed the power depends on the angle


struct LightSource {
    LightSource()
        : m_SourceAngle() {};
public:
    void setSourceAngle(double LightSourceAngle) { m_SourceAngle = LightSourceAngle; };
    void moveSourceAngleBy(double dSourceAngle) { m_SourceAngle += dSourceAngle; };
    double getSourceAngle() const { return m_SourceAngle; };
private:
    double m_SourceAngle;
};

// Exercise 2
// Complete the class LightSource
// It needs an attribute to keep the information about curent angle.
// Setters/getters are trivial.


double LuminationAngle(PanelSetup somesetup, LightSource somelightsource) {
    if(somesetup.getAngle()<0) return pi / 2 - somelightsource.getSourceAngle() + somesetup.getAngle();
    else return pi / 2 + somelightsource.getSourceAngle() - somesetup.getAngle();
}


class SolarPlant : protected PanelSetup {
public:

    //Exercise 3
    // add method allowing to set "fix/insert" the setups into the plant
    // setPanelSetup( where, PanelSetup object)

    //??????????????????????????????????????????????????????????????
    // Is it possible to add array of object to constructor like in the line below?
    //SolarPlant( const PanelSetup (&setupforeach)[10]/* = PanelSetup(0, SolarPanel(20, 30))*/) : m_setups(setupforeach) {} ;
    SolarPlant( const PanelSetup setups[10]) {
        //loop over the arrays
    }
    SolarPlant()=default;
    void setPanelSetup(const PanelSetup& setup, int index) {
        m_setups[index] = setup;
    }
    // Exercise 4
    // add the calculation of the total power produced for a given position of the source
    // it will invole iterating over PanelSetups and summing all the power
    double currentOutput(const LightSource& source) const {
        double output = 0;
        for (int i = 0; i < 10; i++) {
            output += m_setups[i].currentPower(LuminationAngle(m_setups[i], source));
        }
        return output;
    };
    /// This function is compileable, but doesn't work.
    void setNelementXYofaPanel(int nx, int ny, int index) {
        m_setups[index].getPanel().shrinkXto(nx);  m_setups[index].getPanel().shrinkYto(ny);
        cout<<m_setups[index].getPanel().areainCM2() << std::endl;
    }
    void print() /*const*/ { 
        for ( int i =0; i < 10; ++i)
        std::cout << "  " << i  << " angle " << m_setups[i].getAngle() << " panel area " << m_setups[i].getPanel().areainCM2() << std::endl;
    }
private:
    PanelSetup m_setups[10];
};


int main() {
    // For Exercise 1
    PanelSetup testSetup(-pi / 2, SolarPanel(10, 10));
    testSetup.setNPanel(2,3);
    // play with methods like currentPower or efficiency to make sure they work as expected
    cout << testSetup.currentPower(pi / 2) << "; " << testSetup.efficiency(pi) << endl;

    // For remaining exercises
    // construct a default power plant and setup the source to change the angle from -90 to 90 (-pi/2, pi/2)
    // and see the profile of the produced power
    LightSource theSun;
    theSun.setSourceAngle(-pi / 2);
    SolarPlant powerPlant;
    for(int i = 0; i<10; i++){
        powerPlant.setPanelSetup(testSetup, i);
    }
    while (theSun.getSourceAngle() < pi / 2) {
        // calculate and print out of the plant
        // powerPlant.currentOutput( theSun );

        double tmpLumi = LuminationAngle(testSetup, theSun);
        cout << testSetup.currentPower(tmpLumi) << "; " << powerPlant.currentOutput(theSun) << endl;
        theSun.moveSourceAngleBy(pi / 16); // move the sun a bit in each cycle
    }

    // Exercise 5
    // The produced power depends clearly on the position of the Sun
    // Now we would like to have a more optimal plant that produces the energy in a bit more uniform way
    // we want to prepare (insert PanelSetups in this plant) in a special way that not all SolarPanels have the same inclination angle.
    // Something like this below:
    // The \_/ illustrate incidence angles and numbers 1, 2, 3 .. denote positions of the sun during the day (changing angle).
    // At zenith (position 3) the central panels will produce most of the power and in the morning (1) the rightmost panels will produce the most power and the leftmost very little.
    //                                3   
    //                           2         4   
    //                        1               5
    //
    //                       \ \ \ \ _ _ / / / /
    // 
    // construct such a plant and use it in the same kind of loop as above.
    theSun.setSourceAngle(-pi / 2);

    
    for (auto element : { 0,1,2,3 }) {
        powerPlant.setPanelSetup(PanelSetup(pi / 4), element);
        powerPlant.setNelementXYofaPanel(10,10, element); // You can put SolarPanel(x,y) inside the above PanelSetup argument intead.
    }
    for (auto element : { 4,5 }) {
        // Why PanelSetup can use only 1 argument for 2-arguments constructor? Because it was having default arguments set before. The later just overwrite them.
        powerPlant.setPanelSetup(PanelSetup(pi / 2), element);
    }
    for (auto element : { 6,7,8,9 }) {
        powerPlant.setPanelSetup(PanelSetup(-pi / 4), element);
    }
    powerPlant.print();
    // ... 

    while (theSun.getSourceAngle() < pi / 2 + pi/16) {
        cout << "Sun position: " << theSun.getSourceAngle() << "; Current output: " << powerPlant.currentOutput(theSun) << endl;
        theSun.moveSourceAngleBy(pi / 16); // move the sun a bit in each cycle
    }
    // All of the sudden our few classes allow to study quite interesting optimistion problem. 
    // That is how to setup the panels to get a flat energy profile per day. 
    // One may maybe even model how much more power can be produced if panels could rotate? Would it be worth investment ...?

}
