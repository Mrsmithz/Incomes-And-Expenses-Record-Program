#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

int callback(void *notused, int argc, char**argv, char **column);

int main11() {
	sqlite3 *db;
	sqlite3_stmt *res;
	char *err_msg = 0;
	
	int rc = sqlite3_open("test.db", &db);
	if (rc == SQLITE_OK) {
		printf("SQLITE3 OPEN PASSED\n");
	}
	char *sql = "SELECT * FROM \"15/03/2020\"";
				
	rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

	if (rc == SQLITE_OK) {
		printf("VALUE ACCESSED\n");
	}
	return 0;
}
int callback11(void *notused, int argc, char**argv, char **column) {
	notused = 0;
	
	for (int i = 0; i < argc; i++) {
		printf("%s = %s\n", column[i], argv[i] ? argv[i] : "NULL");
	}
	return 0;
}