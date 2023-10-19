#ifndef PHASE_H
#define PHASE_H

class Phase {
  public:
    virtual void enter() = 0;
    virtual Phase* update() = 0;
    virtual void exit() = 0;

    virtual ~Phase() {}
};

#endif
