
#ifndef __NETWORK_H__
#define __NETWORK_H__

/*--------------------------------- Includes ---------------------------------*/

#include "../Thread.h"

/*------------------------------- Primary Class ------------------------------*/

namespace THREAD {

class Network : public OS::Thread<112>
{
public:

protected:
    void Entry(void);
};

} /* namespace THREAD */

/*----------------------------------------------------------------------------*/

#endif /* __NETWORK_H__ */
