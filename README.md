# DBMS Final
###### tags: `DBMS`
## 購物系統架構

這是一套購物系統，有下列 entities
1. 賣家: ID、商店名稱、匯款帳戶  
2. 買家:ID、名字、住所、電話  
3. 商品:ID、品名、價格
4. 訂單:ID
5. 購物清單:ID

且有以下條件

- 由買家建立購物清單，買家有可能會成立購物清單，有可能不將任何購物清單加入至訂單
- 買家可將商品加入至購物清單，每筆購物清單內的商品皆為相同，也須紀錄加入的商品數量
- 由賣家增加商品，但賣家有可能不出售任何商品
- 可將多筆購物清單加入至一筆訂單中
- 可選擇性對交易評價

## ER diagram
![](https://i.imgur.com/XFjLC50.png)



## Relation schema
![](https://i.imgur.com/HYaPyk5.png)


- table & attribute & **relationship**
	- 賣家: ID(primary key)、商店名稱(買家才知道商品是由哪間賣家所提供的)、匯款帳戶 (可將收入金額匯進帳戶中)
	- 買家:ID(primary key)、名字、住所(貨要送至哪裡)、電話(當有問題時可以聯絡)
	- 商品:ID(primary key)、品名、價格(標示品名與價格可讓買家來選擇商品)、**賣家ID(每個商品都有其賣家)**
	- 訂單:ID(primary key)、是否評價、評分(紀錄交易情況)
	- 購物清單:ID(primary key)、數量(要購買多少個)、是否加入訂單(可先留著清單，當決定好時再加入至訂單中)、**買家ID(購物清單為買家所建立的)**、**商品ID(紀錄購物清單內是增加哪一項商品)**、**訂單ID(被加入至哪一筆訂單中)**


## 執行環境

1. Ubuntu 20.04
2. Qt (qtcreator and qt5-default)
3. g++ (程式是用 C++ 撰寫的)

## Database

1. 使用 mysql
2. 需設定 username: ariel, password:aaa
3. 需先建立 database 與 tables
- 建立資料庫，名稱為 DBMS
- 建立 5 個 tables ，分別是商品 、訂單 、買家 、賣家  、購物清單 ，以下是 tables 中 columns 的資訊。
```
mysql> SHOW COLUMNS FROM 商品;
+----------+----------+------+-----+---------+----------------+
| Field    | Type     | Null | Key | Default | Extra          |
+----------+----------+------+-----+---------+----------------+
| ID       | int      | NO   | PRI | NULL    | auto_increment |
| 品名     | char(20) | YES  |     | NULL    |                |
| 價格     | int      | YES  |     | NULL    |                |
| 賣家ID   | int      | YES  | MUL | NULL    |                |
+----------+----------+------+-----+---------+----------------+
4 rows in set (0.01 sec)

mysql> SHOW COLUMNS FROM 訂單;
+--------------+------------+------+-----+---------+----------------+
| Field        | Type       | Null | Key | Default | Extra          |
+--------------+------------+------+-----+---------+----------------+
| ID           | int        | NO   | PRI | NULL    | auto_increment |
| 是否評價     | tinyint(1) | YES  |     | NULL    |                |
| 評分         | int        | YES  |     | NULL    |                |
+--------------+------------+------+-----+---------+----------------+
3 rows in set (0.00 sec)

mysql> SHOW COLUMNS FROM 買家;
+--------+----------+------+-----+---------+----------------+
| Field  | Type     | Null | Key | Default | Extra          |
+--------+----------+------+-----+---------+----------------+
| ID     | int      | NO   | PRI | NULL    | auto_increment |
| 名字   | char(20) | YES  |     | NULL    |                |
| 住所   | char(50) | YES  |     | NULL    |                |
| 電話   | char(20) | YES  |     | NULL    |                |
+--------+----------+------+-----+---------+----------------+
4 rows in set (0.00 sec)

mysql> SHOW COLUMNS FROM 賣家;
+--------------+----------+------+-----+---------+----------------+
| Field        | Type     | Null | Key | Default | Extra          |
+--------------+----------+------+-----+---------+----------------+
| ID           | int      | NO   | PRI | NULL    | auto_increment |
| 商店名稱     | char(10) | YES  |     | NULL    |                |
| 匯款帳戶     | char(20) | YES  |     | NULL    |                |
+--------------+----------+------+-----+---------+----------------+
3 rows in set (0.00 sec)

mysql> SHOW COLUMNS FROM 購物清單;
+--------------------+------------+------+-----+---------+----------------+
| Field              | Type       | Null | Key | Default | Extra          |
+--------------------+------------+------+-----+---------+----------------+
| ID                 | int        | NO   | PRI | NULL    | auto_increment |
| 買家ID             | int        | YES  | MUL | NULL    |                |
| 商品ID             | int        | YES  | MUL | NULL    |                |
| 數量               | int        | YES  |     | NULL    |                |
| 是否加入訂單       | tinyint(1) | YES  |     | NULL    |                |
| 訂單ID             | int        | YES  | MUL | NULL    |                |
+--------------------+------------+------+-----+---------+----------------+
```

## 系統目標

1. 可以搜尋商品
2. 買家可將商品加入至購物清單與新增訂單
3. 可以為訂單評分
4. 搜尋買家的訂單明細

## 使用說明

- mysql 指令輸入
![](https://i.imgur.com/aphIiSK.png)


- 商品查詢
![](https://i.imgur.com/lwrnbdz.png)

- 購物清單
![](https://i.imgur.com/Ez0gWF6.png)


- 訂單查詢與評分
![](https://i.imgur.com/meql3aw.png)

- 訂單明細
![](https://i.imgur.com/rIVWLWK.png)

## Requirement

- Document
	- [x] 系統架構與環境；介面截圖與使用說明
	- [x] ER diagram
	- [x] Relation Schema
	- [x] 5 entities at least ; entity間relationship須具有二元和三元關係
	- [x] 3 attributes at least,key attribute in every table;10 tuples at least in every table
- 使用者操作介面
	- [x] Query: 使用者可以透過介面輸入 SQL 指令
	- [x] Button: 使用者可以透過介面上的元件，執行嵌入的 SQL 指令
- 系統需要使用 SQL 指令
	- [x] SELECT FROM WHERE
	- 使用商品品名查詢，列出商品ID、品名、價格、商店名稱
```
select 商品.ID,品名,價格,商店名稱 from 商品, 賣家 where 品名='洗髮精' and 商品.賣家ID=賣家.ID;
```
- 使用買家ID查詢，列出購物清單ID、品名、商店名稱、是否加入訂單
```
select 購物清單.ID, 品名, 商店名稱, 數量, 是否加入訂單  from 購物清單, 商品, 賣家 where 購物清單.商品ID=商品.ID and 商品.賣家ID=賣家.ID and 買家ID=3;
```
- 使用買家ID查詢, 列出訂單明細
```
select 訂單ID, 品名, 數量, 價格,  數量*價格 as 小計 from 購物清單,商品 where 商品.ID=商品ID and 是否加入訂單=1 and 買家ID=1;
```

```
select 訂單ID, 品名, 數量, 價格,  數量*價格 as 小計 from 購物清單,商品 where 商品.ID=商品ID and 是否加入訂單=1 and 買家ID=1 and 訂單ID in (select 訂單ID from
購物清單,商品 where 商品.ID=商品ID and 是否加入訂單=1 and 買家ID=1 group by 訂單ID having SUM(數量*價格)>0 and SUM(數量*價格)<1000);
```
- [x] DELETE
- 刪除購物清單
```
delete from 購物清單 where ID=(39) ;
```
- [x] INSERT
- 新增至購物清單
```
insert into 購物清單 (買家ID, 商品ID, 數量, 是否加入訂單, 訂單ID) values (6, 7, 3, 0, NULL);
```
- 新增訂單
```
insert into 訂單(是否評價, 評分) values (0, NULL);
```
- [x] UPDATE
- 更新訂單評分
```
update 訂單 set 評分=2 , 是否評價=1 where ID= 2;
```
- [x] IN
-  選取的購物清單更新至訂單
```
update 購物清單 set 是否加入訂單=1, 訂單ID=1 where ID in (50,51);
```
- [x] NOT IN
-  沒有選取的購物清單更新至訂單
```
update 購物清單 set 是否加入訂單=1, 訂單ID=1 where 是否加入訂單=0 and ID not in (8,9);
```
- [x] EXISTS
- 搜尋買家所成立的訂單
```
select * from 訂單 where exists (select * from 購物清單 where 買家ID=1 and 訂單.ID=訂單ID);
```
- [x] NOT EXISTS
-  搜尋除了所選買家所成立的訂單
```
select * from 訂單 where not exists (select * from 購物清單 where 買家ID=1 and 訂單.ID=訂單ID);
```
- [x] COUNT
- 每筆訂單內共有幾筆購物清單
```
select 訂單ID,COUNT(*) as 購物清單數量,SUM(數量*價格) as 總和 from 購物清單,商品 where  商品.ID=商品ID and 是否加入訂單=1 and 買家ID=5 group by 訂單ID;
```
- [x] SUM
-  訂單金額
```
select 訂單ID,SUM(數量*價格) as 總和 from 購物清單,商品 where  商品.ID=商品ID and 是否加入訂單=1 and 買家ID=5 group by 訂單ID;
```
- [x] MAX
- 所有訂單內最大筆的金額
```
select MAX(總和) as 最高金額 from (select 訂單ID,SUM(數量*價格) as 總和 from 購物清單,商品 where 商品.ID=商品ID and 是否加入訂單=1 and 買家ID=5 group by 訂單ID) 訂單總和;
```
- [x] MIN
- 所有訂單內最小筆的金額
```
select MIN(總和) as 最低金額 from (select 訂單ID,SUM(數量*價格) as 總和 from 購物清單,商品 where 商品.ID=商品ID and 是否加入訂單=1 and 買家ID=5 group by 訂單ID) 訂單總和;
```
- [x] AVG
- 所有訂單內平均的金額
```
select AVG(總和) as 平均金額 from (select 訂單ID,SUM(數量*價格) as 總和 from 購物清單,商品 where 商品.ID=商品ID and 是否加入訂單=1 and 買家ID=5 group by 訂單ID) 訂單總和;
```
- [x] HAVING
- 金額介於設定值得訂單
```
select 訂單ID,SUM(數量*價格) as 總和 from 購物清單,商品 where  商品.ID=商品ID and 是否加入訂單=1 and 買家ID=5 group by 訂單ID having 總和>400 and 總和<600;
```
