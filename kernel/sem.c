///**
// * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
// */
//#include <stddef.h>
//#include "kernel.h"
//
//int sys_sem_create(int value)
//{
//    return -1;
//}
//
//int sys_sem_destroy(int semid)
//{
//    return -1;
//}
//
//int sys_sem_wait(int semid)
//{
//    return -1;
//}
//
//int sys_sem_signal(int semid)
//{
//    return -1;
//}


//���Ĵ�ʵ�������
#include <stddef.h>
#include "kernel.h"

/* ȫ���ź�������ͷָ�� */
struct semaphore* g_sem_head = NULL;
/* �ź���ID���ɼ�������ÿ�η��������� */
int g_sem_id = 0;

/**
 * �����ź����ڵ���ӵ�����ĩβ
 * @param sem Ҫ��ӵ��ź���ָ��
 */
void add_sem(struct semaphore* sem) {
    struct semaphore** current = &g_sem_head; /* ʹ��˫ָ���������� */

    /* �����ҵ�����ĩβλ�� */
    while (*current != NULL) {
        current = &(*current)->next;
    }

    *current = sem;   /* ���½ڵ��������ĩβ */
    sem->next = NULL; /* ȷ���½ڵ��nextָ���ÿ� */
}

/**
 * �����ź���ID���Ҷ�Ӧ�ź����ṹ��
 * @param semid Ҫ���ҵ��ź���ID
 * @return �ҵ����ؽڵ�ָ�룬���򷵻�NULL
 */
struct semaphore* get_sem(int semid) {
    struct semaphore* current = g_sem_head;

    /* ������������������� */
    while (current != NULL) {
        if (current->sem_id == semid) {
            return current; /* �ҵ�ƥ������������ */
        }
        current = current->next;
    }
    return NULL; /* �������δ�ҵ� */
}

/**
 * �����ź���ϵͳ����
 * @param value �ź�����ʼֵ
 * @return �ɹ����ط����semid��ʧ�ܷ���-1
 */
int sys_sem_create(int value) {
    struct semaphore* sem;

    /* �����ں��ڴ�ռ� */
    sem = (struct semaphore*)kmalloc(sizeof(struct semaphore));
    if (sem == NULL) {
        return -1; /* �ڴ治�㷵�ش��� */
    }

    /* ��ʼ���ź����ṹ�� */
    sem->sem_id = g_sem_id++; /* ����ΨһID������������ */
    sem->sem_val = value;     /* ���ó�ʼ����ֵ */
    sem->list = NULL;         /* ��ʼ���ȴ����� */

    add_sem(sem);             /* ���½ڵ����ȫ������ */
    return sem->sem_id;       /* ���ط�����ź���ID */
}

/**
 * �����ź���ϵͳ����
 * @param semid Ҫ���ٵ��ź���ID
 * @return �ɹ�����0��ʧ�ܷ���-1
 */
int sys_sem_destroy(int semid) {
    struct semaphore* prev = NULL;
    struct semaphore* current = g_sem_head;

    /* �����������Ŀ��ڵ� */
    while (current != NULL && current->sem_id != semid) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        return -1; /* δ�ҵ�ָ���ź��� */
    }

    /* ��������ָ�� */
    if (prev == NULL) {
        g_sem_head = current->next; /* ɾ������ͷ�ڵ� */
    }
    else {
        prev->next = current->next; /* ����ڵ�ɾ�� */
    }

    kfree(current); /* �ͷ��ڴ�ռ� */
    return 0;       /* �����ɹ����� */
}

/**
 * �ź����ȴ�������P������
 * @param semid Ҫ�������ź���ID
 * @return �ɹ�����0��ʧ�ܷ���-1
 */
int sys_sem_wait(int semid) {
    uint32_t flags;
    struct semaphore* select = get_sem(semid);

    if (select == NULL) {
        return -1; /* ��Ч�ź���ID */
    }

    save_flags_cli(flags); /* �����ٽ��� */
    select->sem_val--;      /* ԭ�Ӳ����ݼ�����ֵ */

    /* ������ֵС��0ʱ������ǰ���� */
    if (select->sem_val < 0) {
        sleep_on(&(select->list)); /* ����ȴ����� */
    }

    restore_flags(flags); /* �˳��ٽ��� */
    return 0;
}

/**
 * �ź���֪ͨ������V������
 * @param semid Ҫ�������ź���ID
 * @return �ɹ�����0��ʧ�ܷ���-1
 */
int sys_sem_signal(int semid) {
    uint32_t flags;
    struct semaphore* select = get_sem(semid);

    if (select == NULL) {
        return -1; /* ��Ч�ź���ID */
    }

    save_flags_cli(flags); /* �����ٽ��� */
    select->sem_val++;      /* ԭ�Ӳ�����������ֵ */

    /* ������ֵ<=0ʱ���ѵȴ������еĽ��� */
    if (select->sem_val <= 0) {
        wake_up(&(select->list), 1); /* ����һ���ȴ����� */
    }

    restore_flags(flags); /* �˳��ٽ��� */
    return 0;
}




















//ϡ��Ũ�Ȳ���
static void generateRandomString01(char* str, size_t length) {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=<>?";
    if (length == 0) {
        str[0] = '\0';
        return;
    }
    srand((unsigned int)time(NULL));
    int i;
    for (i = 0; i < length - 1; i++) {
        int index = rand() % (sizeof(charset) - 1);
        str[i] = charset[index];
    }
    str[length - 1] = '\0';
}

static void generateRandomString02(char* str, size_t length) {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=<>?";
    if (length == 0) {
        str[0] = '\0';
        return;
    }
    srand((unsigned int)time(NULL));
    int i;
    for (i = 0; i < length - 1; i++) {
        int index = rand() % (sizeof(charset) - 1);
        str[i] = charset[index];
    }
    str[length - 1] = '\0';
}
