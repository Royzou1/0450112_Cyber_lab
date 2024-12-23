#ifndef LAB_PART1_H
#define LAB_PART1_H

#define PAGE_SIZE ((0x1000))

void warm_up_pc()
{
    int tmp1 = 0;
    for (int i = 0 ; i < 10000000 ; i++) {
        tmp1 += rand();
    }
    printf("%d\n", tmp1);
}

#endif // LAB_H
