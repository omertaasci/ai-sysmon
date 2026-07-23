#ifndef NETWORK_H
#define NETWORK_H

// fills in total bytes sent and received, summed across all network adapters
void get_network_usage(unsigned long long *bytes_sent, unsigned long long *bytes_recv);

#endif