#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <string.h>
#include <sqlite3.h>
#include <Windows.h>

GtkTreeStore *treestore;
GtkTreeIter toplevel, child;
GtkWidget *income, *expense, *submit_btn, *delete_btn, *sum_btn, *calendar, *note;
GtkWidget *window, *layout, *treeview, *scroll;
GtkTreeModel *model;
GtkTreeViewColumn *col1, *col2, *col3, *col4, *col5;
GtkCellRenderer *cell1, *cell2, *cell3, *cell4, *cell5;
GdkPixbuf *icon;
int count = 0;
char test[100];
double summaryall = 0;
double expenseall = 0;
double incomeall = 0;
static const char *database = "Database.db";
enum
{
	dates = 0,
	notes,
	incomes,
	expenses,
	summary,
	cols
};

static GtkTreeModel *create_model(void);
static GtkWidget *create_tree_view(void);
static void load_ui(void);
static void load_css(void);
static void add_data(void);
static void create_sql(void);
GdkPixbuf *create_pixbuf(const gchar * filename);
int add_data_to_sql(void);
int get_data_from_sql();
int callback(void *notused, int argc, char**argv, char**colname);
char callback2(void *notused, int argc, char**argv, char**colname);
char get_data_from_tree_view();
int delete_row();
int hideconsole();
int get_sum_from_sql();
int callback3(void *notused, int argc, char**argv, char**colname);
int add_sum_to_sql();
void delete_summary_from_sql();
int pop_up();
int keypress_listener(GtkWidget *button, GdkEventKey *event);
int main(int argc, char**argv) {
	gtk_init(&argc, &argv);
	hideconsole();
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), "INCOMES AND EXPENSES RECORDS");
	icon = create_pixbuf("share/icon/Emo01.png");
	gtk_window_set_icon(GTK_WINDOW(window), icon);
	layout = gtk_layout_new(NULL, NULL);
	load_ui();
	gtk_container_add(GTK_WINDOW(window), layout);
	get_data_from_sql();
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
static GtkTreeModel *create_model(void) {

	treestore = gtk_tree_store_new(cols, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

	/*gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel, 0, "Faith", 1, "FAITH2", -1);

	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel, 0, "Faith1", 1, "FAITH3", -1);*/

	return GTK_TREE_MODEL(treestore);
}
static GtkWidget *create_tree_view(void) {

	treeview = gtk_tree_view_new();

	col1 = gtk_tree_view_column_new();
	col2 = gtk_tree_view_column_new();
	col3 = gtk_tree_view_column_new();
	col4 = gtk_tree_view_column_new();
	col5 = gtk_tree_view_column_new();

	gtk_tree_view_column_set_title(col1, "Dates");
	gtk_tree_view_column_set_title(col2, "Notes");
	gtk_tree_view_column_set_title(col3, "Incomes");
	gtk_tree_view_column_set_title(col4, "Expenses");
	gtk_tree_view_column_set_title(col5, "Summary");
	
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col1);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col2);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col3);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col4);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col5);
	
	gtk_tree_view_column_set_fixed_width(col1, 100);
	gtk_tree_view_column_set_fixed_width(col2, 350);
	gtk_tree_view_column_set_fixed_width(col3, 110);
	gtk_tree_view_column_set_fixed_width(col4, 110);
	gtk_tree_view_column_set_fixed_width(col5, 110);
	
	cell1 = gtk_cell_renderer_text_new();
	cell2 = gtk_cell_renderer_text_new();
	cell3 = gtk_cell_renderer_text_new();
	cell4 = gtk_cell_renderer_text_new();
	cell5 = gtk_cell_renderer_text_new();

	gtk_tree_view_column_pack_start(col1, cell1, TRUE);
	gtk_tree_view_column_pack_start(col2, cell2, TRUE);
	gtk_tree_view_column_pack_start(col3, cell3, TRUE);
	gtk_tree_view_column_pack_start(col4, cell4, TRUE);
	gtk_tree_view_column_pack_start(col5, cell5, TRUE);

	gtk_tree_view_column_add_attribute(col1, cell1, "text", dates);
	gtk_tree_view_column_add_attribute(col2, cell2, "text", notes);
	gtk_tree_view_column_add_attribute(col3, cell3, "text", incomes);
	gtk_tree_view_column_add_attribute(col4, cell4, "text", expenses);
	gtk_tree_view_column_add_attribute(col5, cell5, "text", summary);
	
	model = create_model();
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
	g_object_unref(model);
	
	return treeview;
}
static void load_ui(void) {
	GtkWidget *income_label, *expense_label, *note_label;
	load_css();
	
	income_label = gtk_label_new("Incomes: ");
	expense_label = gtk_label_new("Expenses: ");
	note_label = gtk_label_new("Notes: ");

	gtk_widget_set_name(income_label, "income_label");
	gtk_widget_set_name(expense_label, "expense_label");
	gtk_widget_set_name(note_label, "note_label");

	gtk_widget_set_size_request(GTK_LABEL(income_label), 50, 50);
	gtk_widget_set_size_request(GTK_LABEL(expense_label), 50, 50);
	gtk_widget_set_size_request(GTK_LABEL(note_label), 50, 50);

	gtk_layout_put(GTK_LAYOUT(layout), income_label, 230, 315);
	gtk_layout_put(GTK_LAYOUT(layout), expense_label, 225, 365);
	gtk_layout_put(GTK_LAYOUT(layout), note_label, 240, 430);

	income = gtk_entry_new();
	expense = gtk_entry_new();
	note = gtk_entry_new();
	
	gtk_widget_set_name(income, "income_input");
	gtk_widget_set_name(expense, "expense_input");
	gtk_widget_set_name(note, "note_input");
	
	gtk_widget_set_size_request(GTK_ENTRY(income), 400, 50);
	gtk_widget_set_size_request(GTK_ENTRY(expense), 400, 50);
	gtk_widget_set_size_request(GTK_ENTRY(note), 400, 100);

	gtk_entry_set_placeholder_text(GTK_ENTRY(income), "Incomes");
	gtk_entry_set_placeholder_text(GTK_ENTRY(expense), "Expenses");
	gtk_entry_set_placeholder_text(GTK_ENTRY(note), "Notes");

	g_signal_connect(income, "key-press-event", G_CALLBACK(keypress_listener), NULL);
	g_signal_connect(expense, "key-press-event", G_CALLBACK(keypress_listener), NULL);
	g_signal_connect(note, "key-press-event", G_CALLBACK(keypress_listener), NULL);

	gtk_layout_put(GTK_LAYOUT(layout), income, 320, 315);
	gtk_layout_put(GTK_LAYOUT(layout), expense, 320, 365);
	gtk_layout_put(GTK_LAYOUT(layout), note, 320, 415);

	submit_btn = gtk_button_new_with_label("Submit");
	gtk_widget_set_size_request(GTK_BUTTON(submit_btn), 100, 50);
	gtk_layout_put(GTK_LAYOUT(layout), submit_btn, 320, 517);
	g_signal_connect(submit_btn, "clicked", G_CALLBACK(add_data_to_sql), NULL);

	delete_btn = gtk_button_new_with_label("Delete");
	gtk_widget_set_size_request(GTK_BUTTON(delete_btn), 100, 50);
	gtk_layout_put(GTK_LAYOUT(layout), delete_btn, 425, 517);
	g_signal_connect(delete_btn, "clicked", G_CALLBACK(delete_row), NULL);

	sum_btn = gtk_button_new_with_label("Summary");
	gtk_widget_set_size_request(GTK_BUTTON(sum_btn), 100, 50);
	gtk_layout_put(GTK_LAYOUT(layout), sum_btn, 530, 517);
	g_signal_connect(sum_btn, "clicked", G_CALLBACK(get_sum_from_sql), NULL);
	//g_signal_connect(sum_btn, "clicked", G_CALLBACK(get_data_from_tree_view), NULL);

	gtk_widget_set_name(submit_btn, "submit_btn");
	gtk_widget_set_name(delete_btn, "delete_btn");
	gtk_widget_set_name(sum_btn, "sum_btn");

	calendar = gtk_calendar_new();
	gtk_widget_set_size_request(GTK_CALENDAR(calendar), 200, 100);
	gtk_layout_put(GTK_LAYOUT(layout), GTK_CALENDAR(calendar), 10, 315);
	g_signal_connect(calendar, "day-selected", G_CALLBACK(create_sql), NULL);
	
	scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_size_request(GTK_SCROLLED_WINDOW(scroll), 780, 300);
	gtk_layout_put(GTK_LAYOUT(layout), scroll, 10, 10);

	treeview = create_tree_view();
	gtk_tree_view_set_enable_search(treeview, FALSE);
	gtk_widget_set_size_request(GTK_TREE_VIEW(treeview), 770, 300);
	gtk_container_add(GTK_SCROLLED_WINDOW(scroll), treeview);

	gtk_widget_set_name(treeview, "treeview");
}
static void load_css(void) {
	GtkCssProvider *css;
	css = gtk_css_provider_new();
	gtk_css_provider_load_from_path(css, "share/theme.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(css),GTK_STYLE_PROVIDER_PRIORITY_USER);
}
/*static void add_data(void) {
	char a[100], b[100], c[100], d[100], result[100], date_format[100];
	unsigned int day, month, year;
	double income_value, expense_value;
	snprintf(b, sizeof(b), "%s", gtk_entry_get_text(GTK_ENTRY(note)));
	income_value = atof((char *)gtk_entry_get_text(GTK_ENTRY(income)));
	expense_value = atof((char *)gtk_entry_get_text(GTK_ENTRY(expense)));
	snprintf(c, sizeof(c), "%.2lf", income_value);
	snprintf(d, sizeof(d), "%.2lf", expense_value);
	snprintf(result, sizeof(result), "%.2lf", (income_value-expense_value));
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "%02d/%02d/%04d", day, month+1, year);
	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel, dates, date_format, notes, b, incomes, c, expenses, d, summary, result, -1);
	//create_sql();
}*/
sqlite3 *db;
sqlite3_stmt *statement;
char *err_msg = 0;

static void create_sql(void) {
	unsigned int day, month, year;
	char date_format[200], check[200], check2[100];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "CREATE TABLE IF NOT EXISTS \"%02d/%02d/%04d\" (notes TEXT, incomes TEXT, expenses TEXT, result TEXT);", day, month + 1, year);
	snprintf(check, sizeof(check), "SELECT name FROM sqlite_master WHERE type='table' AND name=\"%02d/%02d/%04d\"", day, month + 1, year);
	snprintf(check2, sizeof(check2), "%02d/%02d/%04d", day, month + 1, year);
	int rc = sqlite3_open(database, &db);
	char *sql = &date_format;
	char *sql2 = &check;
	rc = sqlite3_exec(db, sql2, callback2, 0, &err_msg);
	printf("%s\n%s\n", test, check2);
	if (strcmp(test, check2)) {
		rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
		get_data_from_sql();
	}
	else {
		get_data_from_sql();
	}
}
int add_data_to_sql(void) {
	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "INSERT INTO \"%02d/%02d/%04d\" VALUES (?, ?, ?, ?);", day, month + 1, year);
	char a[100], b[100], c[100], d[100], result[100], check1[100], check2[100];
	double income_value, expense_value;
	int len1 = 1, len2 = 1;
	snprintf(check1, sizeof(check1), "%s", gtk_entry_get_text(GTK_ENTRY(income)));
	snprintf(check2, sizeof(check2), "%s", gtk_entry_get_text(GTK_ENTRY(expense)));

	for (int i = 0; i < strlen(check1); i++) {
		len1 *= isdigit(check1[i]);
	}
	for (int j = 0; j < strlen(check2); j++) {
		len2 *= isdigit(check2[j]);
	}
	if (!(len1 * len2)) {
		pop_up();
		return 0;
	}
	else {
		printf("Passed\n");
	}
	
	snprintf(b, sizeof(b), "%s", gtk_entry_get_text(GTK_ENTRY(note)));
	income_value = atof((char *)gtk_entry_get_text(GTK_ENTRY(income)));
	expense_value = atof((char *)gtk_entry_get_text(GTK_ENTRY(expense)));
	snprintf(c, sizeof(c), "%.2lf", income_value);
	snprintf(d, sizeof(d), "%.2lf", expense_value);
	snprintf(result, sizeof(result), "%.2lf", (income_value - expense_value));
	
	int rc = sqlite3_open(database, &db);
	char *sql = &date_format;
	rc = sqlite3_prepare_v2(db, sql, -1, &statement, 0);
	
	sqlite3_bind_text(statement, 1, b, -1, 0);
	sqlite3_bind_text(statement, 2, c, -1, 0);
	sqlite3_bind_text(statement, 3, d, -1, 0);
	sqlite3_bind_text(statement, 4, result, -1, 0);
	
	printf("%s", sql);
	int step = sqlite3_step(statement);
	get_sum_from_sql();
	gtk_entry_set_text(GTK_ENTRY(note), "");
	gtk_entry_set_text(GTK_ENTRY(income), "");
	gtk_entry_set_text(GTK_ENTRY(expense), "");
}
int get_data_from_sql() {
	gtk_tree_store_clear(treestore);
	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "SELECT * FROM \"%02d/%02d/%04d\"", day, month + 1, year);

	int rc = sqlite3_open(database, &db);

	char *sql = &date_format;

	rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
}
int callback(void *notused, int argc, char **argv, char **colname) {
	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "%02d/%02d/%04d", day, month + 1, year);
	
	notused = 0;

	//char buffer[200];
	//snprintf(buffer, sizeof(buffer), "%s%s%s%s", argv[0], argv[1], argv[2], argv[3]);
	//printf("%s\n", buffer);
	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel, 0, date_format, -1);
	
	for (int i = 0; i < argc; i++) {
		gtk_tree_store_set(treestore, &toplevel, i+1, argv[i], -1);
	}
	return 0;
}
char get_data_from_tree_view() {
	GtkTreeIter iter;
	GtkTreePath *path;
	char *str = NULL, *str2 = NULL, *str3 = NULL, *str4 = NULL;
	char buffer[200];
	path = gtk_tree_model_get_path(model, &toplevel);
	if (gtk_tree_model_get_iter(model, &iter, path)) {
		//gtk_tree_model_iter_previous(model, &iter);
		gtk_tree_model_get(model, &iter, 1, &str, 2, &str2, 3, &str3, 4, &str4, -1);
		printf("%s %s %s %s\n", str, str2, str3, str4);
	}
	snprintf(buffer, sizeof(buffer), "%s%s%s%s", str, str2, str3, str4);
	printf("%s", buffer);
	return buffer;
}
char callback2(void *notused, int argc, char**argv, char**colname) {
	notused = 0;
	snprintf(test, sizeof(test), "%s", argv[0]);
	return 0;
}
int delete_row() {
	GtkTreeSelection *select;
	char *note_select = NULL, *income_select = NULL, *expense_select = NULL, *summary_select = NULL;
	select = gtk_tree_view_get_selection(treeview);
	gtk_tree_selection_set_mode(select, GTK_SELECTION_BROWSE);
	if (gtk_tree_selection_get_selected(select, &model, &toplevel)) {
		gtk_tree_model_get(model, &toplevel, 1, &note_select, 2, &income_select, 3, &expense_select, 4, &summary_select, -1);
		printf("%s %s %s %s\n", note_select, income_select, expense_select, summary_select);
	}

	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	//snprintf(date_format, sizeof(date_format), "DELETE FROM \"%02d/%02d/%04d\" WHERE notes=? AND incomes=? AND expenses=? AND result=?;", day, month + 1, year);
	snprintf(date_format, sizeof(date_format), "DELETE FROM \"%02d/%02d/%04d\" WHERE rowid in (select min(rowid) from \"%02d/%02d/%04d\" WHERE notes=? AND incomes=? AND expenses=? AND result=?);", day, month + 1, year, day, month+1, year);
	int rc = sqlite3_open(database, &db);
	char *sql = &date_format;
	printf("%s\n", sql);

	rc = sqlite3_prepare_v2(db, sql, -1, &statement, 0);

	sqlite3_bind_text(statement, 1, note_select, -1, 0);
	sqlite3_bind_text(statement, 2, income_select, -1, 0);
	sqlite3_bind_text(statement, 3, expense_select, -1, 0);
	sqlite3_bind_text(statement, 4, summary_select, -1, 0);

	int step = sqlite3_step(statement);
	get_sum_from_sql();
	//get_data_from_sql();

	return 0;
}
int get_sum_from_sql() {
	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "SELECT incomes, expenses FROM \"%02d/%02d/%04d\" where notes!='Summary'", day, month + 1, year);

	int rc = sqlite3_open(database, &db);

	char *sql = &date_format;

	rc = sqlite3_exec(db, sql, callback3, 0, &err_msg);
	add_sum_to_sql();
}
int callback3(void*notused, int argc, char**argv, char**colname){
	notused = 0;
	for (int i = 0; i < argc; i++) {
		if (!strcmp(colname[i], "incomes")) {
			summaryall += atof(argv[i]);
			incomeall += atof(argv[i]);
		}
		else {
			summaryall -= atof(argv[i]);
			expenseall += atof(argv[i]);
		}
	}
	return 0;
}
int add_sum_to_sql() {
	delete_summary_from_sql();
	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "INSERT INTO \"%02d/%02d/%04d\" VALUES (?, ?, ?, ?);", day, month + 1, year);
	char a[100], b[100], c[100], d[100];
	snprintf(a, sizeof(a), "%.2lf", summaryall);
	snprintf(b, sizeof(b), "%s", "Summary");
	snprintf(c, sizeof(c), "%.2lf", incomeall);
	snprintf(d, sizeof(d), "%.2lf", expenseall);

	int rc = sqlite3_open(database, &db);
	char *sql = &date_format;
	rc = sqlite3_prepare_v2(db, sql, -1, &statement, 0);

	sqlite3_bind_text(statement, 1, b, -1, 0);
	sqlite3_bind_text(statement, 2, c, -1, 0);
	sqlite3_bind_text(statement, 3, d, -1, 0);
	sqlite3_bind_text(statement, 4, a, -1, 0);

	printf("%s", sql);
	int step = sqlite3_step(statement);
	summaryall = 0;
	expenseall = 0;
	incomeall = 0;
	get_data_from_sql();
}
void delete_summary_from_sql() {
	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "DELETE FROM \"%02d/%02d/%04d\" WHERE notes='Summary'", day, month+1, year);
	printf("%s", date_format);
	int rc = sqlite3_open(database, &db);
	char *sql = &date_format;
	rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);

}
int pop_up() {
	GtkWidget *popup, *dialog_label, *action;
	GtkDialogFlags flags = GTK_DIALOG_MODAL;
	dialog_label = gtk_label_new("ONLY NUMBER IN INCOMES AND EXPENSES \n\t\t\tIS ALLOWED");
	popup = gtk_dialog_new_with_buttons("ERROR!!", GTK_WINDOW(window), flags, "OK", GTK_RESPONSE_OK, NULL);
	gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(popup))), dialog_label, TRUE, TRUE, 0);
	gtk_widget_set_size_request(GTK_DIALOG(popup), 200, 200);
	gtk_widget_show_all(popup);
	gint response = gtk_dialog_run(GTK_DIALOG(popup));
	if (response == GTK_RESPONSE_OK) {
		gtk_widget_destroy(popup);
	}
}
GdkPixbuf *create_pixbuf(const gchar *filename) {
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if (!pixbuf) {
		printf("can't load icon\n");
	}
	return pixbuf;
}
int hideconsole(){
	HWND hwnd = GetConsoleWindow();
	//ShowWindow(hwnd, SW_MINIMIZE);
	ShowWindow(hwnd, SW_HIDE);
}
int keypress_listener(GtkWidget *button, GdkEventKey *event) {
	char a[100];
	snprintf(a, sizeof(a), "%s", gdk_keyval_name(event->keyval));
	if (event != NULL && !(strcmp(a, "Return"))) {
		add_data_to_sql();
	}
	else {
		return 0;
	}
}