#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  backtrace();
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sigalarm(void)
{
  int ticks;
  uint64 handler;
  struct proc *p = myproc();
  argint(0,  &ticks);
  argaddr(1, &handler);
  p->interval = ticks;
  p->handler = (void*)handler; 
  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc *p = myproc();
  p->trapframe->epc = p->sepc;
  p->trapframe->ra  = p->sra;
  p->trapframe->sp  = p->ssp;
  p->trapframe->gp  = p->sgp;
  p->trapframe->tp  = p->stp;
  p->trapframe->t0  = p->st0;
  p->trapframe->t1  = p->st1;
  p->trapframe->t2  = p->st2;
  p->trapframe->s0  = p->ss0;
  p->trapframe->s1  = p->ss1;
  p->trapframe->a0  = p->sa0;
  p->trapframe->a1  = p->sa1;
  p->trapframe->a2  = p->sa2;
  p->trapframe->a3  = p->sa3;
  p->trapframe->a4  = p->sa4;
  p->trapframe->a5  = p->sa5;
  p->trapframe->a6  = p->sa6;
  p->trapframe->a7  = p->sa7;
  p->trapframe->s2  = p->ss2;
  p->trapframe->s3  = p->ss3;
  p->trapframe->s4  = p->ss4;
  p->trapframe->s5  = p->ss5;
  p->trapframe->s6  = p->ss6;
  p->trapframe->s7  = p->ss7;
  p->trapframe->s8  = p->ss8;
  p->trapframe->s9  = p->ss9;
  p->trapframe->s10 = p->ss10;
  p->trapframe->s11 = p->ss11;
  p->trapframe->t3  = p->st3;
  p->trapframe->t4  = p->st4;
  p->trapframe->t5  = p->st5;
  p->trapframe->t6  = p->st6;

  p->ishandle = 0;
  return 0;
}