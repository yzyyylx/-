using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using TMPro;
using Unity.Mathematics;
using Unity.VisualScripting;
using Unity.VisualScripting.Antlr3.Runtime.Tree;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public int L, R, U, D, money, max_enemy, need_to_build, tot_enemy, score, tot_blood;
    public float last_time, interval, bullet_speed;
    public bool have_cake, GG;
    public GameObject square;
    public GameObject circle;
    public GameObject turret;
    public GameObject enemy;
    public GameObject bullet;
    public GameObject cake;
    public GameObject cake_pos;
    public GameObject blood;
    public GameObject one_grade;
    public GameObject two_grade;
    public GameObject three_grade;
    public Dictionary<Vector2, GameObject> map;
    public Dictionary<Vector2, GameObject> gameObjects;
    public Dictionary<Vector2, GameObject> turret_pos;
    public Dictionary<Vector2, int> turret_cost;
    public Dictionary<Vector2, int> turret_rank;
    public Dictionary<Vector2, float> turret_range;
    public Dictionary<Vector2, int> turret_attack;
    public Dictionary<Vector2, Dictionary<int, GameObject>> turret_grade;
    public Dictionary<Vector2, GameObject> enemy_pos;
    public Dictionary<Vector2, bool> enemy_get_cake;
    public Dictionary<Vector2, int> enemy_HP;
    public Dictionary<Vector2, int> enemy_st_HP;
    public Dictionary<Vector2, int> enemy_age;
    public Dictionary<Vector2, Vector2> enemy_last_pos;
    public Dictionary<Vector2, int> enemy_message;
    public Dictionary<Vector2, Dictionary<int, GameObject>> enemy_blood;
    public Dictionary<Vector2, GameObject> bullet_pos;
    public Dictionary<Vector2, Vector2> bullet_target;
    public Dictionary<Vector2, int> bullet_attack;

    public GameObject test;
    public GameObject text;
    public GameObject mouse_pos;

    float len(Vector2 u) { return Mathf.Sqrt(u.x * u.x + u.y * u.y); }
    int calc_upgrade(int u) { return u * u * 10; }
    bool judge(Vector2 u) { return u.x >= L && u.x <= R && u.y >= D && u.y <= U && !enemy_pos.ContainsKey(u); }
    string turn(int u)
    {
        if (u==0) return "0";
        string res = "";
        for (; u > 0; u /= 10) res = (char)(u % 10 + '0') + res;
        return res;
    }
    Vector2 Mul(Vector2 u, float v)
    {
        Vector2 res=u;
        res.Set(u.x * v, u.y * v);
        return res;
    }
    int get_blood(Vector2 pos){ return enemy_HP[pos]>=0 ?(tot_blood * enemy_HP[pos] - 1) / enemy_st_HP[pos] + 1:0; }
    void change_blood(Vector2 pos,Vector3 cur_pos)
    {
        int len = get_blood(pos);
        for (int i = 1; i <= len; i++) 
        {
            Vector3 blood_pos = cur_pos + new Vector3((float)i / tot_blood - 0.5f, 0.45f, 0) + new Vector3(0, 0, -2);
            if (!enemy_blood[pos].ContainsKey(i)) enemy_blood[pos][i] = Instantiate(blood, blood_pos, new Quaternion());
            else enemy_blood[pos][i].transform.position = blood_pos;
        }
        for (int i = len + 1; i <= tot_blood; i++)
        {
            if (enemy_blood[pos].ContainsKey(i))
            {
                Destroy(enemy_blood[pos][i]);
                enemy_blood[pos].Remove(i);
            }
        }
    }
    void change_grade(Vector3 pos)
    {
        int cnt = 0, now_grade = turret_rank[pos];
        Vector3 grade_pos = new Vector3(-0.4f, -0.4f, -1);
        foreach (var (i, obj) in turret_grade[pos]) Destroy(obj);
        turret_grade[pos] = new();
        for (; now_grade >= 16; now_grade -= 16)
        {
            turret_grade[pos][++cnt] = Instantiate(three_grade, grade_pos + pos, new Quaternion());
            grade_pos += new Vector3(0.2f, 0, 0);
            if (grade_pos.x > 0.5f) grade_pos.Set(-0.4f, grade_pos.y + 0.2f, -1);
        }
        for (; now_grade >= 4; now_grade -= 4)
        {
            turret_grade[pos][++cnt] = Instantiate(two_grade, grade_pos + pos, new Quaternion());
            grade_pos += new Vector3(0.2f, 0, 0);
            if (grade_pos.x > 0.5f) grade_pos.Set(-0.4f, grade_pos.y + 0.2f, -1);
        }
        for (; now_grade >= 1; now_grade --)
        {
            turret_grade[pos][++cnt] = Instantiate(one_grade, grade_pos + pos, new Quaternion());
            grade_pos += new Vector3(0.2f, 0, 0);
            if (grade_pos.x > 0.5f) grade_pos.Set(-0.4f, grade_pos.y + 0.2f, -1);
        }
    }

    void Start()
    {
        map = new();
        gameObjects = new();
        enemy_pos = new();
        enemy_HP = new();
        enemy_st_HP = new();
        enemy_message = new();
        enemy_get_cake = new();
        enemy_age = new();
        enemy_last_pos = new();
        enemy_blood = new();
        turret_pos = new();
        turret_cost = new();
        turret_rank = new();
        turret_range = new();
        turret_attack = new();
        turret_grade = new();
        bullet_pos = new();
        bullet_target = new();
        bullet_attack = new();

        for (int i = L - 1; i <= R + 1; i++) for (int j = D - 1; j <= U + 1; j++) enemy_message[new Vector2(i, j)] = 0;
        for (int i = L; i <= R; i++)
        {
            for(int j = D; j <= U; j++)
            {
                Vector2 now = new Vector2(i, j);
                map[now] = square;
                gameObjects[now] = Instantiate(square, now, new Quaternion());
            }
        }
        cake_pos = Instantiate(cake, new Vector2(R, D), new Quaternion());
        cake_pos.transform.position += new Vector3(0, 0, -3);
        mouse_pos = Instantiate(square, new Vector2(L-2,U+2), new Quaternion());
        mouse_pos.GetComponent<Renderer>().material.color = Color.gray;
    }

    void Update()
    {
        if (GG)
        {
            text.GetComponent<TextMeshProUGUI>().text = "You Lose!\nScore:\n  "+turn(score);
            return;
        }

        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit2D hit = Physics2D.Raycast(ray.origin,ray.direction);
        GameObject now = hit.collider.gameObject;
        Vector3 now_pos = now.transform.position;
        Vector2 now_pos2 = new Vector2(now_pos.x, now_pos.y);
        if (mouse_pos.transform.position != now_pos)
        {
            mouse_pos.transform.position = now_pos;
            mouse_pos.transform.position += new Vector3(0, 0, -5);
        }
        if (Input.GetMouseButtonUp(0))
        {
            bool flag = false;
            Vector2 tur_pos = new Vector2(L, U);
            foreach (var (pos, ene) in turret_pos)
            {
                if (pos == now_pos2)
                {
                    tur_pos = pos;
                    flag = true;
                    break;
                }
            }
            //upgrade turret
            if (flag)
            {
                if (money >= calc_upgrade(turret_rank[tur_pos]))
                {
                    money -= calc_upgrade(turret_rank[tur_pos]);
                    turret_cost[tur_pos] += calc_upgrade(turret_rank[tur_pos]);
                    if(turret_range[tur_pos]<9.9f)
                        turret_range[tur_pos] += 0.5f;
                    turret_attack[tur_pos] += turret_rank[tur_pos];
                    turret_rank[tur_pos]++;
                    change_grade(turret_pos[tur_pos].transform.position);
                }
            }
            // build turret
            if (money >= need_to_build && !turret_pos.ContainsKey(now_pos2))
            {
                GameObject tur = Instantiate(turret, now_pos, new Quaternion());
                tur.transform.position += new Vector3(0, 0, 4);
                turret_pos[now_pos2] = tur;
                turret_cost[now_pos2] = need_to_build;
                turret_rank[now_pos2] = 1;
                turret_range[now_pos2] = 2;
                turret_attack[now_pos2] = 2;
                turret_grade[now_pos2] = new();
                money -= need_to_build;
                need_to_build += 100;
                change_grade(tur.transform.position);
            }
        }
        else if(Input.GetMouseButtonUp(1))
        {
            bool flag = false;
            Vector2 tur_pos = new Vector2(L, U);
            foreach (var (pos, ene) in turret_pos)
            {
                if (pos == now_pos2)
                {
                    tur_pos = pos;
                    flag = true;
                    break;
                }
            }
            //sale turret
            if(flag)
            {
                money += (int)(turret_cost[tur_pos] * 0.8);
                Destroy(turret_pos[tur_pos]);
                turret_pos.Remove(tur_pos);
                turret_rank.Remove(tur_pos);
                turret_range.Remove(tur_pos);
                turret_attack.Remove(tur_pos);
                turret_cost.Remove(tur_pos);
                need_to_build -= 100;
            }
        }
        //enemy moving
        foreach(var(pos,ene) in enemy_pos)
        {
            ene.transform.position = Mul((pos - enemy_last_pos[pos]),(Time.realtimeSinceStartup - last_time) / interval) + enemy_last_pos[pos];
            ene.transform.position += new Vector3(0, 0, -2);
            if (enemy_get_cake[pos])
            {
                cake_pos.transform.position = ene.transform.position;
                cake_pos.transform.position += new Vector3(0, 0, -1);
            }
            change_blood(pos, ene.transform.position);
        }
        //bullet moving
        Dictionary<Vector2, GameObject> tmp_bullet_pos;
        tmp_bullet_pos = new();
        foreach (var (pos, bul) in bullet_pos) tmp_bullet_pos[pos] = bul;
        foreach (var (pos, bul) in tmp_bullet_pos)
        {
            float delta = Time.realtimeSinceStartup - last_time;
            Vector2 e_pos_e = bullet_target[pos];
            if (!enemy_pos.ContainsKey(e_pos_e))//target have died
            {
                Destroy(bul);
                bullet_attack.Remove(pos);
                bullet_pos.Remove(pos);
                bullet_target.Remove(pos);
                continue;
            }
            Vector2 e_pos = Mul((e_pos_e - enemy_last_pos[e_pos_e]), delta / interval) + enemy_last_pos[e_pos_e];
            if (len(e_pos - pos) < bullet_speed * delta)//hit the target
            {
                score += enemy_HP[e_pos_e];
                enemy_HP[e_pos_e] -= bullet_attack[pos];
                change_blood(e_pos_e, enemy_pos[e_pos_e].transform.position);
                if (enemy_HP[e_pos_e] < 0)//enemy die
                {
                    Destroy(enemy_pos[e_pos_e]);
                    if (enemy_get_cake[e_pos_e])
                    {
                        have_cake = true;
                        cake_pos.transform.position=new Vector3(R, D, -3);
                    }
                    enemy_age.Remove(e_pos_e);
                    enemy_get_cake.Remove(e_pos_e);
                    enemy_pos.Remove(e_pos_e);
                    enemy_last_pos.Remove(e_pos_e);
                    enemy_HP.Remove(e_pos_e);
                    enemy_st_HP.Remove(e_pos_e);
                    enemy_blood.Remove(e_pos_e);
                    money += (int)Mathf.Pow((tot_enemy + 5) / 6, 1.2f);
                }
                else score -= enemy_HP[e_pos_e];
                Destroy(bul);
                bullet_attack.Remove(pos);
                bullet_pos.Remove(pos);
                bullet_target.Remove(pos);
            }
            else
            {
                bul.transform.position = (e_pos - pos) / len(e_pos - pos) * bullet_speed * delta + pos;
                bul.transform.position += new Vector3(0, 0, -3);
            }
        }

        {
            bool flag = false;
            Vector2 tur_pos = new Vector2(L, U);
            foreach (var (pos, tur) in turret_pos)
            {
                if (pos == now_pos2)
                {
                    flag = true;
                    tur_pos = pos;
                    break;
                }
            }
            if (flag)
            {
                text.GetComponent<TextMeshProUGUI>().text = "money:\n  " + turn(money) + "\nlevel:\n  " + turn(turret_rank[tur_pos])+"\nrange:\n  " + turn((int)(turret_range[tur_pos]))+"."+turn((int)(turret_range[tur_pos]*10)%10) +"\nattack:\n  " + turn(turret_attack[tur_pos]) + "\nupgrade:\n  " + turn(calc_upgrade(turret_rank[tur_pos]))+"\nsale:\n  " + turn((int)(turret_cost[tur_pos]*0.8))+"\nscore:\n  "+turn(score);
            }
            else
            {
                text.GetComponent<TextMeshProUGUI>().text = "money:\n  " + turn(money) + "\nbuild:\n  " + turn(need_to_build)+"\nscore:\n  "+turn(score);
            }
        }

        if (Time.realtimeSinceStartup - last_time < interval) return;
        last_time = Time.realtimeSinceStartup;

        //new enemy
        if (enemy_pos.Count < max_enemy && !enemy_pos.ContainsKey(new Vector2(L, U))) 
        {
            tot_enemy++;
            GameObject ene = Instantiate(enemy, new Vector2(L, U), new Quaternion());
            Vector2 st_pos = new Vector2(L, U);
            ene.transform.position = st_pos;
            enemy_pos[st_pos] = ene;
            enemy_HP[st_pos] = enemy_st_HP[st_pos] = (int)(Mathf.Pow(1.1f, (tot_enemy + 5) / 6) * 10);
            enemy_get_cake[st_pos] = false;
            enemy_age[st_pos] = 0;
            enemy_last_pos[st_pos] = st_pos;
            enemy_blood[st_pos] = new();
            change_blood(st_pos, st_pos);
        }

        //move
        Vector2[] dir = new Vector2[8];
        dir[0] = new Vector2(1, 0);
        dir[1] = new Vector2(0, -1);
        dir[2] = new Vector2(-1, 0);
        dir[3] = new Vector2(0, 1);
        for (int i = 4; i < 8; i++) dir[i] = dir[i - 4];
        Dictionary<Vector2, GameObject> tmp_enemy_pos;
        tmp_enemy_pos = new();
        foreach (var (pos, ene) in enemy_pos) tmp_enemy_pos[pos] = ene;
        foreach (var (pos, ene) in tmp_enemy_pos) 
        {
            int best_dir = 0, mx = -1, st;
            if (enemy_get_cake[pos]) st = 2;
            else st = 0;
            for (int k = st; k < st+4; k++) 
            {
                Vector2 new_pos = pos + dir[k];
                if (!judge(new_pos) || new_pos == enemy_last_pos[pos]) continue;
                if (enemy_message[new_pos]>mx)
                {
                    mx = enemy_message[new_pos];
                    best_dir = k;
                }
            }
            if (mx > -1)
            {
                if (best_dir >= 4) best_dir -= 4;
                if (enemy_age[pos] % 5 == 4) 
                {
                    for (int k = best_dir + 1; ; k++) 
                    {
                        Vector2 tmp_pos = pos + dir[k];
                        if (!judge(tmp_pos) || tmp_pos == enemy_last_pos[pos]) continue;
                        best_dir = k;
                        break;
                    }
                }
                Vector2 new_pos = pos + dir[best_dir];
                if (pos == new Vector2(L, U) && enemy_get_cake[pos]) GG = true;
                if (pos == new Vector2(R, D) && have_cake && !enemy_get_cake[pos])
                {
                    enemy_get_cake[new_pos] = true;
                    enemy_HP[pos] = Mathf.Min(enemy_st_HP[pos], enemy_HP[pos] + enemy_st_HP[pos] / 2);
                    have_cake = false;
                }
                else enemy_get_cake[new_pos] = enemy_get_cake[pos];
                enemy_get_cake.Remove(pos);
                enemy_pos[new_pos] = ene; enemy_pos.Remove(pos);
                enemy_age[new_pos] = enemy_age[pos] + 1; enemy_age.Remove(pos);
                enemy_HP[new_pos] = enemy_HP[pos]; enemy_HP.Remove(pos);
                enemy_st_HP[new_pos] = enemy_st_HP[pos]; enemy_st_HP.Remove(pos);
                enemy_last_pos[new_pos]=pos; enemy_last_pos.Remove(pos);
                enemy_blood[new_pos] = enemy_blood[pos]; enemy_last_pos.Remove(pos);
            }
            else enemy_last_pos[pos] = pos;
        }
        foreach(var(pos,ene) in enemy_pos)
        {
            if (enemy_get_cake[pos]) enemy_message[pos] += 5;
            else enemy_message[pos] += 2;
        }

        //aim and launch
        const int INF = 1000000000;
        foreach (var (t_pos, tur) in turret_pos)
        {
            int mn = INF;
            Vector2 target_pos = new Vector2(L, U);
            foreach (var (e_pos, ene) in enemy_pos)
            {
                if (len(t_pos - e_pos) > turret_range[t_pos]) continue;
                if (enemy_HP[e_pos] < mn)
                {
                    mn = enemy_HP[e_pos];
                    target_pos = e_pos;
                }
                if (enemy_get_cake[e_pos])
                {
                    mn = -INF;
                    target_pos = e_pos;
                }
            }
            if (mn == INF) continue;
            if (bullet_pos.ContainsKey(t_pos)) Destroy(bullet_pos[t_pos]);
            bullet_target[t_pos] = target_pos;
            bullet_pos[t_pos] = Instantiate(bullet, t_pos, new Quaternion());
            bullet_pos[t_pos].transform.position += new Vector3(0, 0, -3);
            bullet_attack[t_pos] = turret_attack[t_pos];
        }

        money++;
        for (int i = L - 1; i <= R + 1; i++)
        {
            for (int j = D - 1; j <= U + 1; j++)
            {
                Vector2 pos = new Vector2(i, j);
                if (enemy_message[pos] > 0)
                    enemy_message[pos]--;
            }
        }
    }
}
