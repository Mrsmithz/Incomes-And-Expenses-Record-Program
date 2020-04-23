## โปรแกรมบันทึกรายรับ-รายจ่าย

![Image](https://i.imgur.com/WepDreA.jpg)

### Overview

Income and Expense Record Program is a program for record your income and expense daily or monthly or each year to save money and manage your own money with stability and sustainability.

โปรแกรมบันทึกรายรายรับ-รายจ่ายเป็นโปรแกรมจัดทำขึ้นเพื่อวัตถุประสงค์ให้ผู้ใช้งานสามารถใช้โปรแกรมในการบันทึกรายรับรายจ่ายในแต่ละวันหรือแต่ละเดือนหรือในแต่ละปีได้ เพื่อใช้ในการเก็บออมเงิน และบริหารเงินของตนเองได้อย่างมั่นคงและยั่งยืน

### Advantages

- User can delete some data that they want to delete.

- It's easy to check data of other day.

- User can insert income and expense in the same time.

- This program has Note box to note something that you want.

- This program is designed to be easy to use.

- สามารถลบเพื่อแก้ไขข้อมูลที่ต้องการได้
- สามารถย้อนกลับไปดูข้อมูลของวันก่อนหน้าได้อย่างง่ายดาย
- สามรถกรอกรายรับและรายจ่ายและsubmitพร้อมกันได้เลย
- มีช่องNotesสำหรับจดบันทึก
- โปรแกรมถูกออกแบบมาให้ใช้งานง่าย ไม่ยุ่งยาก

### Disadvantages

- This program has no secure because the account can not be separated and it has no security system in the case that user share the computer with other people.

- User can't change the size of window in this program.

- User can't check summary of month or year.

- Note box has limit of message that can show. 

- ข้อมูลไม่ปลอดภัยเนื่องจากไม่สามารถแยกบัญชีในการใช้งานโปรแกรมของผู้ใช้และไม่มีการล็อครหัสผ่านในกรณีที่มีผู้ใช้งานอื่นใช้คอมพิวเตอร์ร่วมด้วย
- หน้าต่างของโปรแกรมไม่สามาถย่อขยายขนาดได้
- ไม่สามารถดูผลรวมเป็นรายเดือนหรือรายปีได้
- ในช่องNotesไม่สามารถแสดงข้อความทั้งหมดได้หากข้อความมีความยาวเกินไป

### Usage

![Image](https://i.imgur.com/ka1L0OY.jpg)

The program devide to 3 parts 

โปรแกรมจะแบ่งออกเป็น 3ส่วนคือ

![Image](https://i.imgur.com/dGfH7bR.jpg)

### 1.
  The first is part used to show the date, detail of income and expense, amount of income and amount of expenses that users have entered in part 3 and the total amount box.
  
  ส่วนที่ใช้ในการแสดงวันเดือนปีที่บันทึก รายละเอียด จำนวนรายรับและจำนวนรายจ่ายที่ผู้ใช้ได้กรอกไว้ในส่วนที่3 และสุดท้ายคือช่องจำนวนเงินทั้งหมด

### 2.
  Users can choose the date as they wish in this part. 
  
  ส่วนปฎิทินให้ผู้ใช้งานสามราถเลือกวัน เดือน ปีตามที่ตนต้องการจะบันทึกและสามารถดูประวิติที่บันทึกไว้ในแต่ละวันที่ต้องการได้

### 3.

![Image](https://i.imgur.com/V3zAuIe.jpg)

  The part that is used to fill in and show in part1 There are 3channels which are incomes box for inserting income amount, expenses box for inserting expense amount and the last box is notes box that can be used to fill in details that you want. There are 3 buttons below that is submit button is used to press to add more income-expense information and show in part1. The next one is delete button. It use to press to delete income and expense information in part1 by clicking on the history that you want to delete in part1 and pressing the delete button to delete it. The last one is summary button. Users can press this to display the total amount of money in each day. 
  
  ส่วนที่ใช้กรอกข้อมูลและนำไปแสดงบนส่วนที่1 ซึ่งจะมีอยู่3ช่องคือ ช่องIncomesไว้สำหรับใส่จำนวนเงินในส่วนของรายรับ, ช่องExpensesไว้สำหรับใส่จำนวนเงินในส่วนของรายจ่าย และช่องสุดท้ายคือช่องNotesใช้สำหรับกรอกรายละเอียดต่างๆได้ และยังมีอีก3ปุ่มด้านล่างคือ ปุ่มSubmitใช้สำหรับกดเพื่อเพิ่มข้อมูลรายรับ-รายจ่ายขึ้นไปแสดงในส่วนที่1 ต่อมาคือปุ่มDelete ใช้สำหรับกดเพื่อลบข้อมูลรายรับ-รายจ่ายในส่วนที่1 โดยการกดเลือกประวัติที่ต้องการลบในส่วนที่1และกดปุ่มDelete และปุ่มSummaryใช้กดเพื่อให้แสดงผลรวมจำนวนเงินทั้งหมดในแต่ละวัน

### Source Code

```markdown
int main(){
    list* head = NULL;
    head=malloc(sizeof(list));
    if (head==NULL) {
       return 0;
    }
    int num1, temp;
    scanf("%d",&num1);
    list * list_current=head;
    for (int i=0;i<num1;i++) {
      scanf("%d",&temp);
      list_current->num=temp;
      if (i!=num1-1) {
        list_current->next=malloc(sizeof(list));
        list_current = list_current->next;
        }
      else {
          list_current->next=NULL;
        }
    }

    struct list *new_node, *list_c=head, *list_pointer, *tmp = head;
    new_node = (struct list*) malloc(sizeof(list));
    int position;
    scanf("%d %d", &position ,&new_node->num);

    if(position==0){
      new_node -> next=head;
      list_c = new_node;
    }
    else{
      for (int count = 1; count<position&&list_pointer != NULL ; count++){
        tmp = tmp->next;}
        new_node->next = tmp->next;
        tmp->next = new_node;
    }

    while (list_c != NULL) {
        printf("%d ", list_c->num);
        list_c = list_c->next;
      }
  return 0;
}
```

```markdown
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
```

```markdown
int delete_row() {
	GtkTreeSelection *select;
	char *note_select = NULL, *income_select = NULL, *expense_select = NULL, *summary_select = NULL;
	select = gtk_tree_view_get_selection(treeview);
	gtk_tree_selection_set_mode(select, GTK_SELECTION_BROWSE);
	if (gtk_tree_selection_get_selected(select, &model, &toplevel)) {
		gtk_tree_model_get(model, &toplevel, 1, &note_select, 2, &income_select, 3, &expense_select, 4, &summary_select, -1);
		printf("%s %s %s %s\n", note_select, income_select, expense_select, summary_select);
	}
```

```markdown
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
```

### Known bugs

![Image](https://i.imgur.com/l4FzB9a.jpg)

The character's position may be like this image because resolution or size of display.
ตำแหน่งตัวอักษรอาจเป็นดังรูปข้างต้นเนื่องจากความละเอียดหรือขนาดของจอแสดงผล

When user open this program in the first time user may have to wait or submit repeatedly for program to work normally.
การเปิดโปรแกรมในครั้งแรกอาจจะต้องรอหรือทำการsubmitซ้ำหลายๆครั้งเพื่อให้โปรแกรมทำงานตามปกติ
