#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <string.h>
#include <sqlite3.h>

GtkTreeStore *treestore;
GtkTreeIter toplevel, child;
GtkWidget *income, *expense, *submit_btn, *delete_btn, *sum_btn, *calendar, *note;
GtkWidget *window, *layout, *treeview, *scroll;
GtkTreeModel *model;

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
static void add_data_to_sql(void);
int get_data_from_sql();
int callback(void *notused, int argc, char**argv, char**colname);

int main(int argc, char**argv) {
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	layout = gtk_layout_new(NULL, NULL);
	load_ui();
	gtk_container_add(GTK_WINDOW(window), layout);
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
	GtkTreeViewColumn *col1, *col2, *col3, *col4, *col5;
	GtkCellRenderer *cell1, *cell2, *cell3, *cell4, *cell5;
	GtkWidget *treeview;

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
	gtk_layout_put(GTK_LAYOUT(layout), note_label, 230, 430);

	income = gtk_entry_new();
	expense = gtk_entry_new();
	note = gtk_entry_new();

	gtk_widget_set_size_request(GTK_ENTRY(income), 400, 50);
	gtk_widget_set_size_request(GTK_ENTRY(expense), 400, 50);
	gtk_widget_set_size_request(GTK_ENTRY(note), 400, 100);

	gtk_entry_set_placeholder_text(GTK_ENTRY(income), "Incomes");
	gtk_entry_set_placeholder_text(GTK_ENTRY(expense), "Expenses");
	gtk_entry_set_placeholder_text(GTK_ENTRY(note), "Notes");

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

	sum_btn = gtk_button_new_with_label("Summary");
	gtk_widget_set_size_request(GTK_BUTTON(sum_btn), 100, 50);
	gtk_layout_put(GTK_LAYOUT(layout), sum_btn, 530, 517);
	g_signal_connect(sum_btn, "clicked", G_CALLBACK(get_data_from_sql), NULL);

	calendar = gtk_calendar_new();
	gtk_widget_set_size_request(GTK_CALENDAR(calendar), 200, 100);
	gtk_layout_put(GTK_LAYOUT(layout), GTK_CALENDAR(calendar), 10, 315);
	
	scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_size_request(GTK_SCROLLED_WINDOW(scroll), 780, 300);
	gtk_layout_put(GTK_LAYOUT(layout), scroll, 10, 10);

	treeview = create_tree_view();
	gtk_widget_set_size_request(GTK_TREE_VIEW(treeview), 770, 300);
	gtk_container_add(GTK_SCROLLED_WINDOW(scroll), treeview);
}
static void load_css(void) {
	GtkCssProvider *css;
	css = gtk_css_provider_new();
	gtk_css_provider_load_from_path(css, "theme.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(css),GTK_STYLE_PROVIDER_PRIORITY_USER);
}
static void add_data(void) {
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
	create_sql();
}
sqlite3 *db;
sqlite3_stmt *statement;
char *err_msg = 0;

static void create_sql(void) {
	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "CREATE TABLE IF NOT EXISTS \"%02d/%02d/%04d\" (notes TEXT, incomes TEXT, expenses TEXT, result TEXT);", day, month + 1, year);
	
	int rc = sqlite3_open("test.db", &db);
	char *sql = &date_format;
	printf("%s", sql);
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if (rc == SQLITE_OK) {
		printf("%s", sql);
	}
	sqlite3_finalize(statement);
	sqlite3_close(db);
}
static void add_data_to_sql(void) {
	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "INSERT INTO \"%02d/%02d/%04d\" VALUES (?, ?, ?, ?);", day, month + 1, year);
	
	char a[100], b[100], c[100], d[100], result[100];
	double income_value, expense_value;
	snprintf(b, sizeof(b), "%s", gtk_entry_get_text(GTK_ENTRY(note)));
	income_value = atof((char *)gtk_entry_get_text(GTK_ENTRY(income)));
	expense_value = atof((char *)gtk_entry_get_text(GTK_ENTRY(expense)));
	snprintf(c, sizeof(c), "%.2lf", income_value);
	snprintf(d, sizeof(d), "%.2lf", expense_value);
	snprintf(result, sizeof(result), "%.2lf", (income_value - expense_value));
	
	int rc = sqlite3_open("test.db", &db);
	char *sql = &date_format;
	rc = sqlite3_prepare_v2(db, sql, -1, &statement, 0);
	
	sqlite3_bind_text(statement, 1, b, -1, 0);
	sqlite3_bind_text(statement, 2, c, -1, 0);
	sqlite3_bind_text(statement, 3, d, -1, 0);
	sqlite3_bind_text(statement, 4, result, -1, 0);
	
	printf("%s", sql);
	int step = sqlite3_step(statement);
	sqlite3_finalize(statement);
	sqlite3_close(db);
}
int get_data_from_sql() {
	
	unsigned int day, month, year;
	char date_format[200];
	gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
	snprintf(date_format, sizeof(date_format), "SELECT * FROM \"%02d/%02d/%04d\"", day, month + 1, year);

	int rc = sqlite3_open("test.db", &db);

	char *sql = &date_format;

	rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
}
int callback(void *notused, int argc, char **argv, char **colname) {
	notused = 0;
	gtk_tree_store_append(treestore, &toplevel, NULL);
	for (int i = 0; i < argc; i++) {
		gtk_tree_store_set(treestore, &toplevel, i+1, argv[i], -1);
	}
	printf("\n");
	return 0;
}