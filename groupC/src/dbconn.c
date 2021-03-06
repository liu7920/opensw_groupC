#include "common.h"
#include "dbconn.h"

void initDB()
{
		if(mysql_library_init(0, NULL, NULL)){
			printf("mysql_library_init() Error\n");
			exit(1);
		}

		conn = mysql_init(NULL);
		if(conn == NULL){
			printf("mysql_init Error\n");
			exit(1);
		}

		printf("initDB Success\n");
}

MYSQL *connectDB()
{
	//Address, ID, PW, Port
	return mysql_real_connect(conn, "localhost", "user", "test", "game", 3306, NULL, 0);
}

void disconnectDB()
{
	mysql_close(conn);
	printf("DB Disconnect Succes\n");
}

int login_check(char *id, char *pw)
{
	int chk = 0;
	MYSQL_ROW row;
	MYSQL_RES *res_set;

	if(strcmp(id, "admin") == 0) chk = 1;

	char sql[100] = "select Pw from Account where Id = '";

	strcat(sql, id);
	strcat(sql, "' and Pw = '");
	strcat(sql, pw);
	strcat(sql, "';");

	if(mysql_query(conn, sql) != 0) return -1; // SQL Error
	else{
		res_set = mysql_store_result(conn);
		row = mysql_fetch_row(res_set);
		if(row == NULL) return 3; // 로그인 실패
		else if(strcmp(pw, row[0]) == 0){
			if(chk == 1) return 2; // 관리자
			else return 1; // 일반 회원
		}
	}
}

int str_check(char *str)
{ // ID 중복 체크
	char sql[100] = "select Id from Account where Id = '";
	MYSQL_ROW row;
	MYSQL_RES *res_set;

	strcat(sql, str);
	strcat(sql, "';");

	if(mysql_query(conn, sql) != 0) return -1; // SQL Error
	else{
		res_set=mysql_store_result(conn);
		row=mysql_fetch_row(res_set);
		if(row == NULL) return 1; // 중복없음
		else return 0; // 중복
	}
}

void insert_Id_Pw(char *Id, char *Pw)
{
	char sql[100] = "insert into Account values('";
	strcat(sql, Id);
	strcat(sql, "','");
	strcat(sql, Pw);
	strcat(sql, "','0');");

	mysql_query(conn, sql); // 회원가입 ID, PW를 DB에 Insert
}

void insert_Notice(char *str)
{
	char sql[100] = "insert into Notice(Text) values('";

	strcat(sql, str);
	strcat(sql, "');");

	mysql_query(conn, sql); // 공지사항 추가
}

void delete_Notice(char *num)
{
	char sql[100] = "delete from Notice where Num = ";

	strcat(sql, num);
	strcat(sql, ";");

	mysql_query(conn, sql); // 공지사항 삭제
}

void modifiy_admin(char *str)
{
	char sql[100] = "select Pw from Account where Id = 'admin';";
	MYSQL_ROW row;
	MYSQL_RES *res_set;

	if(mysql_query(conn, sql) != 0) return; // SQL Error
	else{
		res_set=mysql_store_result(conn);
		row=mysql_fetch_row(res_set);
		if(row == NULL) return;
		else{
			if(strcmp(row[0], str) == 0){
				char temp[100]="";
				char sql2[100] = "update Account set Pw = '";

				printf("새로운 비밀번호 입력 : ");
				scanf("%s", temp);

				strcat(sql2, temp);
				strcat(sql2, "' where Id = 'admin';");

				mysql_query(conn,sql2);
				printf("Admin 비밀번호 변경 완료\n");
			}
			else printf("기존 비밀번호 불일치\n");
		}
	}
}

void modifiy_user(char *str)
{
	char sql[100] = "select Pw from Account where Id = '";
	MYSQL_ROW row;
	MYSQL_RES *res_set;

	strcat(sql, login_id);
	strcat(sql, "';");

	if(mysql_query(conn, sql) != 0) return; // SQL Error
	else{
		res_set=mysql_store_result(conn);
		row=mysql_fetch_row(res_set);
		if(row == NULL) return;
		else{
			if(strcmp(row[0], str) == 0){
				char temp[100]="";
				char sql2[100] = "update Account set Pw = '";

				printf("새로운 비밀번호 입력 : ");
				scanf("%s", temp);

				strcat(sql2, temp);
				strcat(sql2, "' where Id = '");
				strcat(sql2, login_id);
				strcat(sql2, "';");
				mysql_query(conn,sql2);
				printf("%s 회원님 비밀번호 변경 완료\n", login_id);
			}
			else printf("기존 비밀번호 불일치\n");
		}
	}
}

void leave_user(char *str)
{
	int i;
	char sql[100] = "select Pw from Account where Id = '";
	MYSQL_ROW row;
	MYSQL_RES *res_set;

	strcat(sql, login_id);
	strcat(sql, "';");

	if(mysql_query(conn, sql) != 0) return; // SQL Error
	else{
		res_set=mysql_store_result(conn);
		row=mysql_fetch_row(res_set);
		if(row == NULL) return;
		else if(strcmp(row[0], str) == 0){
			printf("1.회원탈퇴 2.이전으로\n");
			printf("입력 : ");
			scanf("%d",&i);
			if(i==1){
				printf("회원 탈퇴가 완료 되었습니다.\n");
				char sql2[100]="delete from Account where id = '";
				strcat(sql2, login_id);
				strcat(sql2, "';");
				mysql_query(conn, sql2);
			}
			else if(i==2){
				printf("회원 탈퇴가 취소 되었습니다.\n");
			}
			else{
				printf("잘못된 입력입니다.\n");
			}
		}
	}
}

void print_Notice()
{
	char sql[100] = "select Text from Notice;";
	MYSQL_ROW row;
	MYSQL_RES *res_set;

	if(mysql_query(conn, sql) != 0) return; // SQL Error
	else{
		res_set=mysql_store_result(conn);
		row=mysql_fetch_row(res_set);
		while(row != NULL){
			printf("%s\n", row[0]);
			row=mysql_fetch_row(res_set);
		}
	}
}

void print_Notice_admin()
{
	char sql[100] = "select * from Notice;";
	MYSQL_ROW row;
	MYSQL_RES *res_set;

	if(mysql_query(conn, sql) != 0) return; // SQL Error
	else{
		res_set=mysql_store_result(conn);
		row=mysql_fetch_row(res_set);
		while(row != NULL){
			printf("%s. ", row[0]);
			printf("%s\n", row[1]);
			row=mysql_fetch_row(res_set);
		}
	}
}

void user_ClTime_clear(char *str)
{
	char sql[100] = "Update Account set Cl_time = '0' where Id = '";

	strcat(sql, str);
	strcat(sql, "';");

	mysql_query(conn, sql); // 유저 클리어타임 시간 초기화
}

void user_ClTime_Insert(char *str)
{
	char sql[100] = "Update Account set Cl_time = '";

	strcat(sql, str);
	strcat(sql, "' where Id = '");
	strcat(sql, login_id);
	strcat(sql, "';");

	mysql_query(conn, sql);
}

void print_Rank()
{
	char rank[10][100];
	char _rank[10][50];
	char _temp[50];
	char _temp1[100];
	int temp,cu=0, q, w, j=0;
	char sql[100] = "select Id, Cl_time from Account;";
	MYSQL_ROW row;
	MYSQL_RES *res_set;

	printf(">>랭크 보기<<\n");

	if(mysql_query(conn, sql) != 0) return; // SQL Error

	else{
		res_set=mysql_store_result(conn);
		for(q=0;q<10;q++){
			row=mysql_fetch_row(res_set);
			if(row == NULL){
				break;
			}
			else if((strcmp(row[0], "admin") != 0) && (strcmp(row[1], "0") != 0)){
				char __temp[100] = "";
				strcpy(__temp, row[0]);
				strcat(__temp, " ");
				strcat(__temp, row[1]);
				strcpy(rank[j], __temp);
				strcpy(_rank[j], row[1]); /* db의 시간만 넣기*/
				cu++;
				j++;
			}
		}

		for(q=0;q<cu;q++){
			for(w=1;w<cu;w++){
				if(strcmp(_rank[q],_rank[w]) > 0 ){
					//시간만 정렬
					strcpy(_temp," ");
					strcpy(_temp,_rank[q]);
					strcpy(_rank[q]," ");
					strcpy(_rank[q],_rank[w]);
					strcpy(_rank[w]," ");
					strcpy(_rank[w],_temp);
					//아이디 + 시간 들어가있는것 정렬
					strcpy(_temp1," ");
					strcpy(_temp1,rank[q]);
					strcpy(rank[q]," ");
					strcpy(rank[q],rank[w]);
					strcpy(rank[w]," ");
					strcpy(rank[w],_temp1);
				}
			}
		}
		printf("***** 랭킹 *****\n");
		for(int i=0;i<cu;i++){
			printf("%d등 %15s\n",i+1, rank[i]);
		}
	}
}
