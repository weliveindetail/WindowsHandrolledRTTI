#include "stdio.h"
#include "WindowsRttiLiveBuildHelpers.h"

struct PreparedBase
{
    static int getStatic() { return 0; }
    virtual int get() { return getStatic(); }
};

struct PreparedDerived : public PreparedBase
{
    static int getStatic() { return 1; }
    int get() override { return getStatic(); }
};

struct UnpreparedDerived : public PreparedBase
{
};

struct UnpreparedAutonomous
{
};

int main()
{
    const char* msg_tmpl = "expected: %d, actual: %d\n";

    PreparedBase prep1;             // JUCE base class
    PreparedDerived prep2;          // JUCE class derived from JUCE class
    UnpreparedDerived unprep1;      // user class derived from JUCE class
    UnpreparedAutonomous unprep2;   // arbitrary user class

    printf(msg_tmpl, 1, WindowsRtti::isa<PreparedBase>(&prep1) ? 1 : 0);
    printf(msg_tmpl, 0, WindowsRtti::isa<PreparedBase>(&prep2) ? 1 : 0);
    printf(msg_tmpl, 0, WindowsRtti::isa<PreparedDerived>(&prep1) ? 1 : 0);
    printf(msg_tmpl, 1, WindowsRtti::isa<PreparedDerived>(&prep2) ? 1 : 0);
    printf("\n");

    // problem: user class derived from JUCE class returns JUCE class type ID.. which is usually wrong
    printf(msg_tmpl, 0, WindowsRtti::isa<PreparedBase>(&unprep1) ? 1 : 0);      
    printf(msg_tmpl, 0, WindowsRtti::isa<PreparedDerived>(&unprep1) ? 1 : 0);
    printf(msg_tmpl, 0, WindowsRtti::isa<UnpreparedDerived>(&unprep1) ? 1 : 0);
    printf("\n");

    // working: correct result and compiler warning for each of them
    printf(msg_tmpl, 0, WindowsRtti::isa<PreparedBase>(&unprep2) ? 1 : 0);
    printf(msg_tmpl, 0, WindowsRtti::isa<PreparedDerived>(&unprep2) ? 1 : 0);
    printf(msg_tmpl, 0, WindowsRtti::isa<UnpreparedDerived>(&unprep2) ? 1 : 0);

    getchar();
    return 0;
}

