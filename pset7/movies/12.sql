select movies.title from movies
join stars on movies.id = stars.movie_id
join people on stars.person_id = people.id
-- where movies.title like 'Sweeney Todd%'
where people.name in ('Johnny Depp', 'Helena Bonham Carter')
group by movies.title
having count(people.name) = 2;