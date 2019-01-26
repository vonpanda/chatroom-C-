#include "client.h"

int main()
{
    client cl;
    cl.init();
    if(cl.connects()==-1)
    	return 0;
    cl.fork_both();
    return 0;
}
