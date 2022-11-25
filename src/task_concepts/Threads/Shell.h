/**
 * @file Shell.h
 * @author James Bennion-Pedley
 * @brief Brief summary here
 * @date 04/11/2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SHELL_H__
#define __SHELL_H__

/*--------------------------------- Includes ---------------------------------*/

#include "../Thread.h"

/*------------------------------- Primary Class ------------------------------*/

namespace THREAD {

class Shell : public OS::Thread<512>
{
public:

protected:
    void Init(void);
    void Entry(void);
};

} /* namespace THREAD */

/*----------------------------------------------------------------------------*/

#endif /* __SHELL_H__ */
