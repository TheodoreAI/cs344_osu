#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



// With this program I can get the IP addresses of the input domain names:
// This will be useful for certain applications and to check where my servers are running.

// This is the following format of the hostentries:

// #include <netdb.h>
// struct hostent {
//     char  *h_name;            /* official name of host */
//     char **h_aliases;         /* an array of alternate names, terminated by a null pointer */
//     int    h_addrtype;        /* host address type. One of AF_INET or AF_INET6 at present. */
//     int    h_length;          /* length of address */
//     char **h_addr_list;       /* an array of addresses, terminated by a null pointer */
// }

int main(int argc, char *argv[]) {
  // Check usage & args
  if (argc != 2) { 
    fprintf(stderr,"USAGE: %s $hostname \n", argv[0]); 
    fprintf(stderr,"Example: %s flip.engr.oregonstate.edu\n", argv[0]);
    exit(0); 
  }

  // Get the DNS entry associated with the domain name
  struct hostent *hostptr = gethostbyname(argv[1]);
  

  if(hostptr == NULL){
     printf("Couldn't locate %s\n", argv[1]);
     exit(0);
  }

  // We successfully looked up the DNS entry
  printf("Official domain name: %s\n", hostptr->h_name);

  // Print all aliases for this domain name
  int i = 0;
  printf("Aliases:\n");
  while(hostptr->h_aliases[i] != NULL){
    printf("\t%s\n", hostptr->h_aliases[i]);
    i++;
  }

  // Print all IP addresses, in dotted decimal notation, for this domain name
  int j = 0;
  printf("Addresses:\n");
  struct in_addr addr;
  while(hostptr->h_addr_list[j] != NULL){
    addr.s_addr = *(unsigned int *) hostptr->h_addr_list[j];
    printf("\t%s\n", inet_ntoa(addr));
    j++;
  }
  return 0;
}