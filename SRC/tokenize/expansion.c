/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:16:22 by mbamatra          #+#    #+#             */
/*   Updated: 2024/09/02 08:20:18 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

char *ft_strjoinchar(char *str, char c)
{
    char *result;
    int len;

    if (!str)
        return NULL;

    len = ft_strlen(str);
    result = malloc(len + 2);
    if (!result)
        return NULL;

    ft_strcpy(result, str);
    result[len] = c;
    result[len + 1] = '\0';
    return result;
}


char *expansion(char *str, int *j, t_data *data, t_quotes *inside_quotes)
{
    t_env *env = data->envp_list;
    char *variable;
    char *result;
    int k = *j + 1;
    if (str[k] == ' ' || str[k] == '\0' || str[k] == '$' || inside_quotes->single_quote)
    {
        (*j)++;
        return ft_strdup("$");
    }
    while (str[k] != ' ' && str[k] != '$' && str[k] != '\0' && str[k] != '\'' && str[k] != '\"')
        k++;
    variable = ft_substr(str, *j + 1, k - *j - 1);
    while (env)
    {
        if (ft_strcmp(variable, env->key) == 0)
        {
            result = ft_strdup(env->value);
            free(variable);
            *j = k;
            return result;
        }
        env = env->next;
    }
    free(variable);
    *j = k;
    return ft_strdup("");
}

void expand_files(t_files *files, t_data *data)
{
    int i;
    char *expanded;
    t_files   *temp;
    t_quotes inside_quotes;
    int j;
    temp = files;
    while (files)
    {
        j = 0;
        char *str = files->file_name;
        char *result = ft_strdup("");
        char *temp;
        initialize_quote(&inside_quotes);
        i = 0;
        while (str[i])
        {
            update_quotes(&inside_quotes, str[i]);
            if (str[i] == '$' && inside_quotes.single_quote == 0)
            {
                // printf("inside expansion\n");
                int start = i;
                expanded = expansion(str, &i, data, &inside_quotes);
                temp = ft_strjoin(result, expanded);
                free(result);
                result = temp;
                free(expanded);
            }
            else
            {
                temp = ft_strjoinchar(result, str[i]);
                free(result);
                result = temp;
                i++;
            }
        }
        free(files->file_name);
        files->file_name = result;
        j++;
        files = files->next;
    }
    files = temp;
}

void expand_variables(t_cmd **cmd, t_data *data)
{
    int i;
    char *expanded;
    t_cmd   *temp;
    t_quotes inside_quotes;
    int j;
    temp = *cmd;
    while (*cmd)
    {
        j = 0;
        while (((*cmd)->content) && (*cmd)->content[j])
        {
            char *str = (*cmd)->content[j];
            char *result = ft_strdup("");
            char *temp;
            initialize_quote(&inside_quotes);
            i = 0;
            while (str[i])
            {
                update_quotes(&inside_quotes, str[i]);
                if (str[i] == '$' && inside_quotes.single_quote == 0)
                {
                    // printf("inside expansion\n");
                    int start = i;
                    expanded = expansion(str, &i, data, &inside_quotes);
                    temp = ft_strjoin(result, expanded);
                    free(result);
                    result = temp;
                    free(expanded);
                }
                else
                {
                    temp = ft_strjoinchar(result, str[i]);
                    free(result);
                    result = temp;
                    i++;
                }
            }
            free((*cmd)->content[j]);
            (*cmd)->content[j] = result;
            j++;
        }
        expand_files((*cmd)->files, data);
        *cmd = (*cmd)->next;
    }
    *cmd = temp;
}
// char *expand_variables(char *str, t_data *data)
// {
//     int i;

//     i = 0;
//     char *expanded;
//     char *temp;
//     char *result = ft_strdup("");
//     t_quotes inside_quotes;
//     initialize_quote(&inside_quotes);
//         while (str[i])
//         {
//             update_quotes(&inside_quotes, str[i]);

//             if (str[i] == '$' && inside_quotes.single_quote == 0)
//             {
//                 int start = i;
//                 expanded = expansion(str, &i, data, &inside_quotes);
//                 temp = ft_strjoin(result, expanded);
//                 free(result);
//                 result = temp;
//                 free(expanded);
//             }
//             else
//             {
//                 temp = ft_strjoinchar(result, str[i]);
//                 free(result);
//                 result = temp;
//                 i++;
//             }
//         }
//     return result;
// }
