#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class ReactorAsset {
protected:
    string unitID;
    bool isOnline;

public:
    static int activeReactors;

    ReactorAsset(string id) : unitID(id), isOnline(true) {
        activeReactors++;
    }

    virtual ~ReactorAsset() {
        activeReactors--;
    }

    virtual void transmitCoreReport() = 0;
};

int ReactorAsset::activeReactors = 0;

class CoolantLoopModule : virtual public ReactorAsset {
protected:
    double coolantFlowRateLps;

public:
    CoolantLoopModule(string id, double flowRate) : ReactorAsset(id), coolantFlowRateLps(flowRate) {}

    void checkCoolantLoop() {
        cout << "Primary coolant [" << unitID << "] flow rate: " << coolantFlowRateLps << " L/s." << endl;
    }
};

class ControlRodModule : virtual public ReactorAsset {
protected:
    double insertionDepthPct;

public:
    ControlRodModule(string id, double depth) : ReactorAsset(id), insertionDepthPct(depth) {}

    void checkControlRods() {
        cout << "Control rods [" << unitID << "] insertion depth: " << insertionDepthPct << "%." << endl;
    }
};

class PressurizedWaterReactor : public CoolantLoopModule, public ControlRodModule {
private:
    double thermalOutputMW;

public:
    PressurizedWaterReactor(string id, double flowRate, double depth, double output)
        : ReactorAsset(id),
          CoolantLoopModule(id, flowRate),
          ControlRodModule(id, depth),
          thermalOutputMW(output) {}

    ~PressurizedWaterReactor() {
        cout << "Reactor " << unitID << " scrammed and safely shut down." << endl;
    }

    void transmitCoreReport() override {
        cout << "\n--- Pressurized Water Reactor Report: " << unitID << " ---" << endl;
        checkCoolantLoop();
        checkControlRods();
        cout << "Thermal Power Output: " << fixed << setprecision(1) << thermalOutputMW << " MW" << endl;
        cout << "Containment Status: Nominal" << endl;
    }
};

int main() {
    ReactorAsset* core = new PressurizedWaterReactor("PWR-CORE-03", 18500.0, 62.5, 1150.0);

    core->transmitCoreReport();

    cout << "\nActive Reactor Units: " << ReactorAsset::activeReactors << endl;

    delete core;

    cout << "Active Units After Shutdown: " << ReactorAsset::activeReactors << endl;

    return 0;
}
