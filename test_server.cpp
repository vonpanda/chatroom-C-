#include<iostream>
#include "server.h"

int main()
{
    server st;
    st.init_ser();
    st.server_run();
    return 0;
}
