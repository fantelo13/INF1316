#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define EVER ;;
#include <sys/wait.h>

void childhandler(int signo);
int delay;

int main (int argc, char *argv[])
{
pid_t pid;
signal(SIGCHLD, childhandler);
if ((pid = fork()) < 0)
{
fprintf(stderr, "Erro ao criar filho\n");
exit(-1);
}
if (pid == 0) /* child */
  execvp(argv[2], &argv[2]); /* ou sleep(3); ou, no próximo exercício, execvp(argv[2], argv);*/
else /* parent */
{
sscanf(argv[1], "%d", &delay); /* read delay from command line */
sleep(delay);
printf("Program %s exceeded limit of %d seconds!\n", argv[2], delay);
kill(pid, SIGKILL);
}
return 0;
}

void childhandler(int signo) /* Executed if child dies before parent */
{
int status;
pid_t pid = wait(&status);
printf("Child %d terminated within %d seconds com estado %d.\n", pid, delay, status);
exit(0);
}








//a- for(EVER) /* filho em loop eterno */

//filho vai entrar em loop eterno e o pai vai matar o filho quando acabar o tempo especificado no delay

//b- sleep(3) /* filho dorme 3 segundos */

//filho vai dormir por 3 segundos e o pai vai matar o filho quando acabar o tempo especificado no delay

//c- execvp(sleep5) /* filho executa o programa sleep5 */

//filho vai executar o programa sleep5 q faz com que ele de sleep 5 e o pai vai matar o filho quando acabar o tempo especificado no delay

//d- execvp(sleep15) /* filho executa o programa sleep15 */

//filho vai executar o programa sleep15 q faz com que ele de sleep 15 e o pai vai matar o filho quando acabar o tempo especificado no delay