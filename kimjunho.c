/* 구현한 소스들 */
#include <stdio.h>
#include <dirent.h>
#include <ftw.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void voiddevkim(char **cline)
{
    if (strcmp(*cline, "cp") == 0)
    {
        if (voidcp(cline) == 0) {
        } else {
            printf("Copy Failed\n");
        }
    }
    else if (strcmp(*cline, "clear") == 0)
    {
        printf("%c[2J", 27);
    }
    else if (strcmp(*cline, "cd") == 0)
    {
        if (voidcd(cline) == 0) {
        } else {
            printf("Failed\n");
        }
    }
    else if (strcmp(*cline, "chmod") == 0)
    {
        if (voidchmod(cline) == 0) {
        } else{
            printf("Error\n");
        }
    }
    else if (strcmp(*cline, "rm") == 0)
    {
        if (voidrm(cline) == 0) {
        } else {
            printf("rm Command Error\n");
        }
    }
    else if (strcmp(*cline, "mv") == 0)
    {
        if (voidmv(cline) == 0) {
        } else {
            printf("mv Command Error\n");
        }
    }
    else if (strcmp(*cline, "head") == 0)
    {
        if (voidhead(cline) == 0) {
        } else {
            printf("head Command Error\n");
        }
    }
    else if (strcmp(*cline, "tail") == 0)
    {
        if (voidtail(cline) == 0) {
        } else {
            printf("tail Command Error\n");
        }
    }
}

voidpwd(cline)
char **cline;
{
    char buf[BUFSIZ];
    getcwd(buf, BUFSIZ);
    printf("%s\n", buf);
    return 0;
}

voidtail(cline)
char **cline;
{
    char temp[BUFSIZ];
    static FILE *fp;
    int count = 0;
    unsigned long long pos;
    if (isOptions(cline) == 0) {
        switch (cline[1][1])
        {
            case 'n':
                if ((fp = fopen(cline[3], "rt")) == NULL)
                {
                    perror("fopen\n");
                    return -1;
                }
                fseek(fp, 0, SEEK_END);
                pos = ftell(fp);
                while(pos) {
                    if (fseek(fp, --pos, SEEK_SET) == 0) {
                        if (fgetc(fp) == '\n') { 
                            if (count++ == atoi(cline[2])) break;
                        }
                    } else {
                        perror("fseek() failed");
                    }
                }
                while(fgets(temp, sizeof(temp), fp) != NULL) {
                    printf("%s", temp);
                }
                fclose(fp);
                return 0;
                break;
            case 'c':
                if ((fp = fopen(cline[3], "rt")) == NULL)
                {
                    perror("fopen\n");
                    return -1;
                }
                fseek(fp, -atoi(cline[2]), SEEK_END);
                while(fgets(temp, sizeof(temp), fp) != NULL) {
                    printf("%s", temp);
                }
                fclose(fp);
                return 0;
                break;
            default:
                printf("-- 사용 가능한 옵션 --\n");
                printf("'-n' : 파일을 라인 수 만큼 읽는다.\n");
                printf("'-c' : 파일을 바이트 만큼 읽는다.\n");
                return 0;
                break;
        }
    } else {
        if ((fp = fopen(cline[1], "rt")) == NULL)
        {
            perror("fopen\n");
            return -1;
        }
        fseek(fp, 0, SEEK_END); // 파일 포인터 맨 마지막으로 이동함.
        pos = ftell(fp); //파일 전체 크기를 알아내서
        while(pos) {
            if (fseek(fp, --pos, SEEK_SET) == 0) { //뒤에서부터 계속 차례대로 확인함
                if (fgetc(fp) == '\n') { 
                    if (count++ == 10) break;
                }
            } else {
                perror("fseek() failed");
            }
        }
        
        while(fgets(temp, sizeof(temp), fp) != NULL) {
            printf("%s", temp);
        }
        fclose(fp);
        return 0;
    }
    return 0;
}

voidhead(cline)
char **cline;
{
    char temp[BUFSIZ];
    static FILE *fp;
    int count = 0;
    if (isOptions(cline) == 0) {
        switch (cline[1][1])
        {
            case 'n': // for line
                if ((fp = fopen(cline[3], "rt")) == NULL)
                {
                    perror("fopen\n");
                    return -1;
                }
                while (fgets(temp, sizeof(temp), fp) != NULL && count < atoi(cline[2]))
                {
                    printf("%s", temp);
                    count++;
                }
                fclose(fp);
                return 0;
                break;
            case 'b': // for byte[]
                if ((fp = fopen(cline[3], "rt")) == NULL)
                {
                    perror("fopen\n");
                    return -1;
                }
                fread(temp, atoi(cline[2]), 1, fp);
                printf("%s", temp);
                fclose(fp);
                return 0;
                break;
            default:
            printf("-- 사용 가능한 옵션 --\n");
            printf("'-n' : 파일을 라인 수 만큼 읽는다.\n");
            printf("'-e' : 파일을 바이트 수 만큼 읽는다.\n");
            return 0;
            break;
        }
    } else {
        if ((fp = fopen(cline[1], "rt")) == NULL)
        {
            perror("fopen\n");
            return -1;
        }
        while (fgets(temp, sizeof(temp), fp) != NULL && count < 10)
        {
            printf("%s", temp);
            count++;
        }
        fclose(fp);
    }
    return 0;
}

voidcat(cline) char **cline;
{
    char temp[BUFSIZ];
    static FILE *fp;
    int count = 0;

    if (isOptions(cline) == 0)
    {
        switch (cline[1][1])
        {
        case 'n':
            if ((fp = fopen(cline[2], "rt")) == NULL)
            {
                perror("fopen\n");
                return -1;
            }
            while (fgets(temp, sizeof(temp), fp) != NULL)
            {
                printf("%d : %s", count++, temp);
            }
            fclose(fp);
            return 0;
            break;
        case 'e':
            if ((fp = fopen(cline[2], "rt")) == NULL)
            {
                perror("fopen\n");
                return -1;
            }
            while (fgets(temp, sizeof(temp), fp) != NULL)
            {
                temp[strlen(temp) - 1] = '$';
                printf("%s\n", temp);
            }
            fclose(fp);
            return 0;
            break;
        default:
            printf("-- 사용 가능한 옵션 --\n");
            printf("'-n' : 파일내용 앞에 줄 번호를 표시한다.\n");
            printf("'-e' : 개행문자를 $로 표시한다.\n");
            return 0;
            break;
        }
    }
    else
    {
        if ((fp = fopen(cline[1], "rt")) == NULL)
        {
            perror("fopen\n");
            return -1;
        }
        while (fgets(temp, sizeof(temp), fp) != NULL)
        {
            printf("%s", temp);
        }
        fclose(fp);
    }
    return 0;
}

rm_default(cline, backup) char **cline;
int backup;
{
    //파일을 cp해서 지우는 것
    char *cpline[BUFSIZ];
    char *rmline[BUFSIZ];
    cpline[0] = "cp";
    cpline[1] = cline[1];
    cpline[2] = cline[2];
    rmline[0] = "rm";
    rmline[1] = cline[1];
    if (voidcp(cpline) == -1)
    {
        return -1;
    }
    if (backup == 1) // 백업이면 이거먼저 처리하고
    {
        char *backupstring = (char *)malloc(strlen(cline[1]) + strlen("_backup") + 1); //현재 파일에 _backup을 붙이고
        strcat(backupstring, cline[1]);
        strcat(backupstring, "_backup");
        cpline[1] = cline[1];
        cpline[2] = backupstring;
        voidcp(cpline); //인자를 이렇게 줘서 복사
    }
    if (voidrm(rmline) == -1) // 삭제함
    {
        return -1;
    }
    return 0;
}

voidmv(cline) char **cline;
{
    // 옵션이 있으면
    if (isOptions(cline) == 0)
    {
        char *rm_data[BUFSIZ];
        int nResult;
        int yesoryes;
        // mv -b abc cba 일 경우
        rm_data[0] = cline[0]; // mv를 저장하고
        rm_data[1] = cline[2]; // abc를 저장
        rm_data[2] = cline[3]; // cba를 저장해서
        //options!!
        switch (cline[1][1])
        {
        case 'b': //백업 후 저장
            if (rm_default(rm_data, 1) == -1)
                return -1;
            return 0;
            break;
        case 'i': // 파일이 있으면 덮어쓸때 물어보는거 
            nResult = access(rm_data[2], 0); // 파일이 있으면
            if (nResult == 0)
            {
                printf("파일이 존재합니다. 덮어 쓰시겠습니까 ? (1 == yes) or (0 == no) >> ");
                scanf("%d", &yesoryes);
                if (yesoryes == 0)
                {
                    return 0;
                }
                else
                {
                    if (rm_default(rm_data, 0) == -1)
                        return -1;
                    return 0;
                }
            }
            if (rm_default(rm_data, 0) == -1)
                return -1;
            return 0;
            break;
        case 'f': // 걍 삭제후 저장
            if (rm_default(rm_data, 0) == -1)
                return -1;
            return 0;
            break;
        default:
            printf("-- 사용 가능한 옵션 --\n");
            printf("'-b' : 지정 위치에 동일 파일이 있을 경우 백업 후 이동한다.\n");
            printf("'-f' : 지정 위치에 동일 파일이 있을 경우 덮어 쓸때 묻지 않는다.\n");
            printf("'-i' : 지정 위치에 동일 파일이 있을 경우 덮어 쓸때 묻는다.\n");
            break;
        }
    }
    else
    {
        if (rm_default(cline, 0) == -1)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

rmri(fpath)
    const char *fpath;
{
    DIR *dp;
    struct dirent *dent;
    struct stat buf;
    char path[BUFSIZ];
    int delete;

    if ((dp = opendir(fpath)) == NULL)
    {
        perror("opendir:");
        exit(1);
    }

    while ((dent = readdir(dp)) != NULL)
    {
        if (dent->d_name[0] == '.')
        {
            continue;
        }
        else
        {
            sprintf(path, "%s", dent->d_name);
            stat(path, &buf);
            if (S_ISDIR((unsigned int)buf.st_mode))
            {
                rmri(dent->d_name);
            }
            else
            {
                printf("%s file is deleted ?? { (1 == yes) or ( 0 == no ) } >> ", dent->d_name);
                scanf("%d", &delete);
                if (delete != 0 && delete != 1)
                    return -1;
                //unlink로 삭제하기.
            }
        }
    }
    printf("\n");
    closedir(dp);
    return 0;
}

rmrf(fpath, sb, tflag, ftwbuf)
    const char *fpath;
const struct stat *sb;
int tflag;
struct FTW *ftwbuf;
{
    switch (tflag)
    {
    case FTW_D:
    case FTW_DP:
        if (rmdir(fpath) == -1)
            perror("unlink");
        break;
    case FTW_F:
    case FTW_SL:
        if (unlink(fpath) == -1)
            perror("unlink");
        break;
    default:
        puts("do nothing");
    }
    return 0;
}

isOptions(cline) char **cline;
{
    if (cline[1][0] == '-')
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

voidrm(cline) char **cline;
{
    if (isOptions(cline) == 0)
    {
        if (strcmp(cline[1], "-rf") == 0)
        {
            int flags = 0;
            if (cline[2] == NULL)
            {
                return -1;
            }
            flags |= FTW_DEPTH; // 하위 디렉토리의 내용을 지우고 처리
            if (nftw(cline[2], rmrf, 10, flags) == -1) // POSIX 함수
            {
                perror("nftw");
                return -1;
            }
            return 0;
        }
        else if (strcmp(cline[1], "-ri") == 0)
        {
            if (rmri(cline[2]) == -1)
            {
                return -1;
            }
            return 0;
        }
        else
        {
            printf("-- 사용 가능한 옵션 --\n");
            printf("'-rf' : 디렉토리와 파일을 전부 삭제한다.\n");
            printf("'-ri' : 디렉토리에 있는 내용을 하나 하나 확인하면서 삭제한다.\n");
            return 0;
        }
    }
    else
    {
        if (unlink(cline[1]) == -1)
        {
            perror("unlink");
            return -1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

voidchmod(cline) char **cline;
{
    struct stat buf;
    int mode = 0;
    char *delim = "+-=";
    char *op1, *op2, operation[1];
    int arr[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP,
                  S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH}; //정보
    int i, j, result;
    switch (cline[1][0])
    {
    case '0': // 숫자로 바꾸는 경우
        stat(cline[1], &buf);
        sscanf(cline[1], "0%o", &mode);
        if (chmod(cline[2], mode) == 0)
        {
            return 0;
        }
        else
        {
            return -1;
        }
        break;
    default: // 문자로 바꾸는 경우
        if (stat(cline[2], &buf) == -1)
        {
            return -1;
        }
        operation[0] = strpbrk(cline[1], delim)[0];
        operation[1] = '\0';
        op1 = strtok(cline[1], delim);
        op2 = strtok(NULL, delim);
        for (i = 0; op1[i] != '\0'; i++)
        {
            for (j = 0; op2[j] != '\0'; j++)
            {
                result = 0;
                if (op1[i] == 'u')
                {
                    result = 0;
                }
                else if (op1[i] == 'g')
                {
                    result = 3;
                }
                else
                {
                    result = 6;
                }

                if (op2[j] == 'r')
                {
                    result += 0;
                }
                else if (op2[j] == 'w')
                {
                    result += 1;
                }
                else
                {
                    result += 2;
                }

                if (operation[0] == '+')
                {
                    buf.st_mode |= arr[result];
                }
                else
                {
                    buf.st_mode &= ~(arr[result]);
                }
            }
        }
        chmod(cline[2], buf.st_mode);
        stat(cline[2], &buf);
        return 0;
        break;
    }
}

voidcd(cline) char **cline;
{
    if (chdir(cline[1]) < 0) // cd 명령어 호출
    {
        return -1;
    }
    system("pwd"); //현재위치 출력
    return 0;
}

voidcp(cline) char **cline;
{
    FILE *rfp, *wfp;
    char buf[BUFSIZ];

    if ((rfp = fopen(cline[1], "r")) == NULL) // 읽기
    {
        perror("fopen");
        return -1;
    }
    if ((wfp = fopen(cline[2], "w")) == NULL) // 쓰기
    {
        perror("fopen");
        return -1;
    }

    while (fgets(buf, BUFSIZ, rfp) != NULL) // 복사
    {
        fputs(buf, wfp);
    }

    fclose(rfp);
    fclose(wfp);
    return 0;
}