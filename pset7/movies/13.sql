select name from people
where id in (
    select distinct(people.id) from movies
    join stars on movies.id = stars.movie_id
    join people on stars.person_id = people.id
    where movies.id in (
        select movies.id from movies
        join stars on movies.id = stars.movie_id
        join people on stars.person_id = people.id
        where people.name = 'Kevin Bacon'
        and people.birth = 1958
    )
    and people.name != 'Kevin Bacon'
);